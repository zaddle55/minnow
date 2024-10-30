#include "byte_stream.hh"
#include <deque>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : 
  capacity_( capacity ), 
  buffer_() {}

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

  for (auto c : data)
  {
    if (available == 0) break;
    buffer_.push_back( c );
    ++has_pushed_;
    --available;
  }
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - ((has_pushed_ >= has_popped_) ? has_pushed_ - has_popped_ : 0);
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return has_pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return buffer_.empty() && closed_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return has_popped_;
}

string_view Reader::peek() const
{
  // Your code here.
  if (buffer_.empty()) return {};
  buffer_str_ = string().assign( buffer_.begin(), buffer_.end() );
  return std::string_view( buffer_str_ );
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  if (error_ ) return;
  while (!buffer_.empty() && len > 0)
  {
    buffer_.pop_front();
    ++has_popped_;
    --len;
  }
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return (has_pushed_ >= has_popped_) ? has_pushed_ - has_popped_ : 0;
}
