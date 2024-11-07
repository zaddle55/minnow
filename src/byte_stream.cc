#include "byte_stream.hh"
#include <deque>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : 
  capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_;
}

void Writer::push( string data )
{
  // Your code here.
  if (error_ || closed_) return;
  uint64_t available = available_capacity();
  if (available == 0 || data.empty()) return;

  data = data.substr( 0, min( data.size(), available ) );
  has_pushed_ += data.size();
  has_buffered_ += data.size();

  buffer_.append( data );

}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - has_buffered_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return has_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return has_buffered_ == 0 && closed_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return has_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  if (error_ || has_buffered_ == 0) return {};
  return string_view( buffer_ );
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (error_ || len == 0 || has_buffered_ == 0) return;
  uint64_t to_pop = min( len, bytes_buffered() );
  has_popped_ += to_pop;
  has_buffered_ -= to_pop;
  buffer_ = buffer_.substr( to_pop );
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return has_buffered_;
}
