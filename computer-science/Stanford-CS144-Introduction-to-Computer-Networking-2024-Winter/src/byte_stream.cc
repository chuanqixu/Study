#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_;
}

void Writer::push( string data )
{
  // Your code here.
  uint64_t avail_cap = available_capacity(), len = min( avail_cap, data.length() );
  if ( len == 0 || is_closed() )
    return;

  if ( data.length() < avail_cap ) {
    buffer_.push_back( std::move( data ) );
  } else {
    buffer_.push_back( data.substr( 0, avail_cap ) );
  }

  bytes_buffered_ += len;
  bytes_count_write_ += len;
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - bytes_buffered_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_count_write_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return buffer_.empty() && closed_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_count_read_;
}

string_view Reader::peek() const
{
  // Your code here.

  // it is complicated for the string_view return version of peek to return multiple bytes with the deque
  // implementation since string_view is just the view of the string

  // if create a new string and return, the new string will be destroyed when the function returns, so string_view
  // will have the stack-use-after-return error

  if ( !buffer_.empty() ) {
    return std::string_view( buffer_.front() );
  } else {
    return std::string_view();
  }
}

void Reader::pop( uint64_t len )
{
  // Your code here.

  // not necessary since if is_finished() is true, len = 0
  // if ( is_finished() )
  //   return;

  len = min( bytes_buffered_, len );
  bytes_count_read_ += len;
  bytes_buffered_ -= len;

  while ( len > 0 ) {
    uint64_t front_length = buffer_.front().length();
    if ( len < front_length ) {
      buffer_.front().erase( 0, len );
      return;
    } else {
      len -= front_length;
      buffer_.pop_front();
    }
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return bytes_buffered_;
}
