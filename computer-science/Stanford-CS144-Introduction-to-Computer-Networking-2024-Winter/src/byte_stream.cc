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
  if ( is_closed() )
    return;

  uint64_t len = available_capacity() < data.length() ? available_capacity() : data.length();
  if ( len == 0 )
    return;

  // prevent overflow
  for ( uint64_t i = 0; i < len - 1; ++i ) {
    data_.push_back( data[i] );
  }
  data_.push_back( data[len - 1] );

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
  return capacity_ - data_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_count_write_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return data_.empty() && closed_;
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

  if ( !data_.empty() ) {
    return std::string_view( &data_.front(), 1 );
  } else {
    return std::string_view();
  }
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if ( is_finished() )
    return;

  len = data_.size() < len ? data_.size() : len;

  // prevent overflow
  if ( len == 0 )
    return;
  for ( uint64_t i = 0; i < len - 1; ++i ) {
    data_.pop_front();
  }
  data_.pop_front();

  bytes_count_read_ += len;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return data_.size();
}
