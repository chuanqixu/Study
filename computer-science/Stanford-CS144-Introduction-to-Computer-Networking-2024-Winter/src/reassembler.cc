#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.

  // below remember not to use '-' for unsigned numbers to prevent overflow
  Writer& writer = output_.writer();
  if ( is_last_substring && last_byte_index == uint64_t( -1 ) )
    last_byte_index = first_index + data.length();

  // 1. check all cases: make sure that data is within the capacity and first index is not less than next_index_

  // (1) if the starting point is larger than the capacity
  if ( first_index >= next_index_ + writer.available_capacity() )
    return;

  // (2) if the length is larger than capacity, shorten the data
  if ( first_index + data.length() > next_index_ + writer.available_capacity() )
    data = data.substr( 0, writer.available_capacity() + next_index_ - first_index );

  // (3) already wrote
  if ( first_index + data.length() <= next_index_ ) {
    // zero length data means the last substring
    if ( next_index_ == last_byte_index )
      writer.close();
    return;
  }

  // (4) partly wrote before
  if ( first_index < next_index_ ) {
    uint64_t i = next_index_ - first_index;
    data = data.substr( i, data.length() - i );
    first_index = next_index_;
  }

  // (5) data is empty
  if ( data.length() == 0 ) {
    // zero length data means the last substring
    if ( next_index_ == last_byte_index )
      writer.close();
    return;
  }

  SubString substr { first_index, data };

  // 2. combine substrings in buffer_: make sure that data and first_index is correct and can be written to writer
  // or be inserted into buffer_, i.e., no overlap in with substrings in buffer_

  // iter->first >= first_index
  auto iter = buffer_.lower_bound( substr );
  auto prev = iter;
  if ( iter != buffer_.begin() )
    --prev;

  // whether can be concatenated with the previous substring or with the current substring
  if ( iter != buffer_.begin() && prev->first_index + prev->data.length() >= first_index ) {
    // data is not fully covered by prev->data, then data need to be updated; otherwise, nothing needs to be done
    if ( prev->first_index + prev->data.length() < first_index + data.length() )
      data = prev->data.substr( 0, first_index - prev->first_index ) + data;
    else
      return;
    bytes_pending_ -= prev->data.length();
    first_index = prev->first_index;
    buffer_.erase( prev );
  }

  // whether can be concatenated with the following substrings (there may be multiple substrings connected by the
  // new substring) remove all data in the buffer that is fully covered by the data to write
  while ( iter != buffer_.end() && first_index + data.length() >= iter->first_index + iter->data.length() ) {
    bytes_pending_ -= iter->data.length();
    iter = buffer_.erase( iter );
  }
  // concatenate the last overlap substring
  if ( iter != buffer_.end() && first_index + data.length() >= iter->first_index ) {
    bytes_pending_ -= iter->data.length();
    data = data.substr( 0, iter->first_index - first_index ) + iter->data;
    buffer_.erase( iter );
  }

  // 3. data and first string is the correct one to add
  if ( next_index_ == first_index ) { // can be directly written to writer
    writer.push( data );
    next_index_ += data.length();
  } else { // cannot be directly written to writer and must be stored in reassembler
    bytes_pending_ += data.length();
    substr.first_index = first_index;
    substr.data = data;
    buffer_.insert( substr );
  }

  if ( next_index_ == last_byte_index )
    writer.close();
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return bytes_pending_;
}
