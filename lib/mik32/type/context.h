#pragma once

template<typename R, typename W, typename S>
class IContext {
public:
  virtual void init() = 0;
  virtual void enable() = 0;
  virtual void disable() = 0;
  // virtual void clear() = 0;
  virtual R read() = 0;
  virtual void write(W) = 0;
};
