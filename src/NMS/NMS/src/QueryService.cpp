/**
 * Autogenerated by Thrift Compiler (0.9.2)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "QueryService.h"

namespace similarity {


ReplyEntry::~ReplyEntry() throw() {
}


void ReplyEntry::__set_id(const int32_t val) {
  this->id = val;
}

void ReplyEntry::__set_dist(const double val) {
  this->dist = val;
}

void ReplyEntry::__set_externId(const std::string& val) {
  this->externId = val;
}

void ReplyEntry::__set_obj(const std::string& val) {
  this->obj = val;
}

void swap(ReplyEntry &a, ReplyEntry &b) {
  using ::std::swap;
  swap(a.id, b.id);
  swap(a.dist, b.dist);
  swap(a.externId, b.externId);
  swap(a.obj, b.obj);
}

ReplyEntry::ReplyEntry(const ReplyEntry& other0) {
  id = other0.id;
  dist = other0.dist;
  externId = other0.externId;
  obj = other0.obj;
}

ReplyEntry& ReplyEntry::operator=(const ReplyEntry& other1) {
  id = other1.id;
  dist = other1.dist;
  externId = other1.externId;
  obj = other1.obj;
  return *this;
}
} // namespace

