// Copyright (c) 2011 David Björklund
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "./binding.h"

#include <node_buffer.h>
#include <node_version.h>
#include <snappy.h>

#include <string.h>  // memcpy

#include <string>

namespace nodesnappy {

v8::Handle<v8::Value> Compress(const v8::Arguments& args) {
    v8::HandleScope scope;
    std::string *input;
    if (node::Buffer::HasInstance(args[0]->ToObject())) {
        v8::Handle<v8::Object> object = args[0]->ToObject();
        size_t length = node::Buffer::Length(object);
        const char *data = node::Buffer::Data(object);
        input = new std::string(data, length);
    } else {
        v8::String::Utf8Value param1(args[0]->ToString());
        input = new std::string(*param1);
    }

    std::string dst;
    snappy::Compress(input->data(), input->length(), &dst);
    delete input;
    node::Buffer *buf = node::Buffer::New(dst.size());
    memcpy(node::Buffer::Data(buf), dst.c_str(), dst.size());

    return scope.Close(buf->handle_);
}

v8::Handle<v8::Value> Uncompress(const v8::Arguments& args) {
    v8::HandleScope scope;
    std::string *input;
    if (node::Buffer::HasInstance(args[0]->ToObject())) {
        v8::Handle<v8::Object> object = args[0]->ToObject();
        size_t length = node::Buffer::Length(object);
        const char *data = node::Buffer::Data(object);
        input = new std::string(data, length);
    } else {
        v8::String::Utf8Value param1(args[0]->ToString());
        input = new std::string(*param1);
    }

    std::string dst;
    if (!snappy::Uncompress(input->data(), input->length(), &dst)) {
        delete input;
        return v8::Local<v8::Value>::New(
                v8::ThrowException(v8::Exception::Error(
                        v8::String::New("snappy uncompress error"))));
    }

    node::Buffer *buf = node::Buffer::New(dst.size());
    memcpy(node::Buffer::Data(buf), dst.c_str(), dst.size());
    delete input;
    
    return scope.Close(buf->handle_);
}

v8::Handle<v8::Value> isValidCompressed(const v8::Arguments& args) {
    v8::HandleScope scope;
    std::string *input;
    if (node::Buffer::HasInstance(args[0]->ToObject())) {
        v8::Handle<v8::Object> object = args[0]->ToObject();
        size_t length = node::Buffer::Length(object);
        const char *data = node::Buffer::Data(object);
        input = new std::string(data, length);
    } else {
        v8::String::Utf8Value param1(args[0]->ToString());
        input = new std::string(*param1);
    }

    bool res = snappy::IsValidCompressedBuffer(input->data(), input->length());
    delete input;
    return scope.Close(v8::Boolean::New(res));
}

extern "C" void
init(v8::Handle<v8::Object> exports) {
  NODE_SET_METHOD(exports, "compress", Compress);
  NODE_SET_METHOD(exports, "uncompress", Uncompress);
  NODE_SET_METHOD(exports, "isValidCompressed", isValidCompressed);
}

NODE_MODULE(binding, init)
}  // namespace nodesnappy
