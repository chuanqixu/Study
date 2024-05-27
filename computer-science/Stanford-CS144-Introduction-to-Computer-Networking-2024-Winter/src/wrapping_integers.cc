#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return Wrap32 { zero_point + n };
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  const uint64_t checkpoint_higher32 = checkpoint & 0xffffffff00000000;
  const uint64_t sqeno = ( raw_value_ - zero_point.raw_value_ ) | checkpoint_higher32;
  uint64_t half = 1ULL << 31;

  if ( checkpoint > sqeno && checkpoint - sqeno > half ) {
    half <<= 1;
    return sqeno + half;
  } else if ( checkpoint < sqeno && sqeno - checkpoint > half ) {
    if ( checkpoint_higher32 == 0 )
      return sqeno;
    half <<= 1;
    return sqeno - half;
  }

  return sqeno;
}
