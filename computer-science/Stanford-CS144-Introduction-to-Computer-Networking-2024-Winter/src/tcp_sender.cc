#include "tcp_sender.hh"
#include "tcp_config.hh"

#include <string_view>

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  // Your code here.
  return sequence_numbers_in_flight_;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  // Your code here.
  return n_consecutive_retransmissions_;
}

void TCPSender::push( const TransmitFunction& transmit )
{
  // Your code here.
  uint16_t window_size = window_size_;
  if ( window_size == 0 )
    window_size = 1;

  Reader& r = input_.reader();
  if ( sequence_numbers_in_flight_ >= window_size ) {
    return;
  }

  uint64_t available_bytes = window_size - sequence_numbers_in_flight_;
  bool more_data = true;
  while ( available_bytes > 0 && more_data ) { // for big window, payload must be divided into small chunks
    // read as many bytes as possible from reader
    uint64_t bytes_popped = r.bytes_popped();
    uint64_t bytes_to_send = min( available_bytes, TCPConfig::MAX_PAYLOAD_SIZE ), curr = 0;
    string payload( bytes_to_send, 0 );
    while ( bytes_to_send > 0 ) {
      string_view str = r.peek();
      if ( str.length() == 0 ) {
        payload.erase( curr );
        more_data = false;
        break;
      }

      uint64_t len = min( str.length(), bytes_to_send );
      if ( len == str.length() ) {
        payload.replace( curr, len, str );
      } else {
        payload.replace( curr, len, str.substr( 0, len ) );
        payload.erase( curr + len );
      }
      r.pop( len );
      curr += len;
      bytes_to_send -= len;
    }

    // set the message to send
    bool syn = bytes_popped == 0 && !sent_syn_,
         fin = r.is_finished() && !sent_fin_
               && payload.length() < available_bytes; // should not add FIN if window is full
    // notice that the last condition is not bytes_to_send != 0, since MAX_PAYLOAD_SIZE only limits payload

    const TCPSenderMessage message {
      Wrap32::wrap( bytes_popped + sent_syn_, isn_ ), syn, payload, fin, input_.has_error() };
    if ( syn ) {
      sent_syn_ = true;
    }
    if ( fin ) {
      sent_fin_ = true;
    }

    // update outstanding and set timer
    if ( message.sequence_length() ) {
      outstanding_.push( { r.bytes_popped() + sent_syn_ + sent_fin_, message } );
      sequence_numbers_in_flight_ += message.sequence_length();
      transmit( message );
      if ( !timer_running_ ) {
        timer_running_ = true;
        if ( RTO_ms_ == 0 )
          RTO_ms_ = initial_RTO_ms_;
      }
    }

    available_bytes -= curr;
  }
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  // Your code here.
  return { Wrap32::wrap( reader().bytes_popped() + sent_syn_ + sent_fin_, isn_ ), 0, "", 0, input_.has_error() };
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  // Your code here.
  window_size_ = msg.window_size;
  if ( msg.RST ) {
    input_.set_error();
    return;
  }

  // update outstanding
  uint64_t outstanding_size = outstanding_.size();
  while ( !outstanding_.empty() ) {
    auto [outstanding_ack_abs_seq, message] = outstanding_.front();
    uint64_t ack_abs_seq = msg.ackno.value().unwrap( isn_, outstanding_ack_abs_seq );
    if ( reader().bytes_popped() + sent_syn_ + sent_fin_
         < ack_abs_seq ) { // Impossible ackno (beyond next seqno) is ignored
      return;
    }

    if ( outstanding_ack_abs_seq > ack_abs_seq ) { // the first outstanding segment is not received
      break;
    } else { // the first outstanding segment is acknowledged
      sequence_numbers_in_flight_ -= message.sequence_length();
      outstanding_.pop();
    }
  }

  // update timer
  if ( outstanding_size != outstanding_.size() ) { // successful receipt of new data
    // reset retransmission
    RTO_ms_ = initial_RTO_ms_;
    n_consecutive_retransmissions_ = 0;
    time_ms_ = 0;
  }
  if ( outstanding_.empty() ) {
    timer_running_ = false;
  }
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  // Your code here.
  if ( !timer_running_ )
    return;

  time_ms_ += ms_since_last_tick;
  if ( time_ms_ >= RTO_ms_ && !outstanding_.empty() ) {
    transmit( outstanding_.front().second );
    if ( window_size_ ) {
      ++n_consecutive_retransmissions_;
      RTO_ms_ *= 2;
    }
    time_ms_ = 0;
  }
}
