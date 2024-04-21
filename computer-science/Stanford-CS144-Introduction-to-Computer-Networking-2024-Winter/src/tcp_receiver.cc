#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  uint64_t abs_seqno;
  if ( message.RST ) {
    reader().set_error();
    return;
  } else if ( !message.SYN ) {
    if ( isn_.has_value() ) // have connected before
      abs_seqno = message.seqno.unwrap( isn_.value(), writer().bytes_pushed() );
    else // have not connected yet
      return;
  } else { // SYN is set
    isn_ = Wrap32( message.seqno + 1 );
    abs_seqno = 0;
  }

  reassembler_.insert( abs_seqno, std::move( message.payload ), message.FIN );

  // FIN may be sent before all strings can be reassembled
  // in this case, the message may be out of order, and in the following previous bytes may be receive
  // so the condition is to check whether writer is finished (writer is finished when having received FIN and
  // reassembling all strings)
  if ( writer().is_closed() )
    isn_ = isn_.value() + 1;
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  const Writer& w = writer();
  return { isn_.has_value() ? Wrap32::wrap( w.bytes_pushed(), isn_.value() ) : isn_,
           static_cast<uint16_t>( w.available_capacity() > UINT16_MAX ? UINT16_MAX : w.available_capacity() ),
           w.has_error() };
}
