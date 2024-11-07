#include "reassembler.hh"

using namespace std;

#include <iostream>

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  // Your code here.

  auto aval = available_capacity();
  data = data.substr(0, min(data.size(), aval));
  if (next_index_ >= first_index) // data can be write into bytestream with overlap
  {
    data = data.substr(min(next_index_ - first_index, data.size()));
    output_.writer().push(data);
    next_index_ += data.size();

    // start to load possibly in-order substrings to bytestream
    for (auto it = buffer_.begin(); it != buffer_.end(); ++it)
    {
      if (it->first + it->second.size() <= next_index_)
      {
        bytes_pending_ -= it->second.size();
        it = buffer_.erase(it);
      }
      else if (it->first <= next_index_)
      {
        output_.writer().push(it->second.substr(next_index_ - it->first));
        next_index_ = it->first + it->second.size();
        bytes_pending_ -= it->second.size();
        it = buffer_.erase(it);
      }
      else
      {
        break;
      }
    }
  }
  else // to resemble the out-of-order substrings
  {
    uint64_t expect_add = data.size();
    uint64_t sl = first_index, sr = first_index + data.size();

    for (auto it = buffer_.begin(); it != buffer_.end(); ++it)
    {
      uint64_t tl = it->first, tr = it->first + it->second.size();
      if (sr < tl) // if substring wait to be pended is not overlap with the current substring
      {
        buffer_.emplace(it, first_index, data);
        break;
      }
      else if (sl > tr) // if substring wait to be pended is not overlap with the current substring
      {
        // if iterator is the last element in the buffer
        if (next(it) == buffer_.end()) { buffer_.emplace_back(first_index, data); }
        continue;
      }
      else // if substring wait to be pended is overlap with the current substring
      {
        cout << "expect_add = " << expect_add << endl;
        expect_add = max(sl - tl, (uint64_t)0) + max(tr - sr, (uint64_t)0);
        cout << "expect_add = " << expect_add << endl;
        if (tl >= sl ) { data.replace( tl - sl , tr - tl , it->second ); }
        else { data = it->second + data.substr(tr - sl); }
        sl = min(sl, tl);
        sr = max(sr, tr);
        it = buffer_.erase(it);
      }
    }
    if (buffer_.empty()) 
    {
      buffer_.emplace_back(first_index, data);
    }
    bytes_pending_ += expect_add;
    cout << "bytes_pending_ = " << bytes_pending_ << endl;
  }

  if (is_last_substring)
  {
    output_.writer().close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return bytes_pending_;
}
