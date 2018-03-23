/**
 * 表达式模板
 * @user 任师锋
 * Link: https://github.com/dmlc/mshadow/blob/master/guide/exp-template/README.md
 * */

/**
 * 如何高效的写出ML代码？
 * 比如，weight =  - eta * (grad + lambda * weight);
 * 其中，grad, weight都是n维的向量
 * 1. 如何高效的计算？
 *    预申请内存，避免临时内存的申请！例如
 *    void UpdateWeight (const float *grad, float eta, float lambda, int n, float *weight) {
          for (int i = 0; i < n; ++i) {
            weight[i] =  - eta * (grad[i] + lambda * weight[i]);
          }
      }

  2. 这种方式不直观，有没有高效的方式实现数学上的直观操作呢？
  void UpdateWeight (const Vec& grad, float eta, float lambda, Vec& weight) {
    weight = -eta * (grad + lambda * weight);
  }
 *
 */
#include <iostream>
#include <cstdio>
#include <cstdlib>

//Naive solution for vector operation overloading
struct Vec {
  int len;
  float* dptr;
  Vec(int len) : len(len) {
    dptr = new float[len];
  }
  Vec(const Vec& src) : len(src.len) {
    dptr = new float[len];
    memcpy(dptr, src.dptr, sizeof(float)*len );
  }
  ~Vec(void) {
    delete [] dptr;
  }
};

inline Vec operator+(const Vec &lhs, const Vec &rhs) {
  Vec res(lhs.len);
  for (int i = 0; i < lhs.len; ++i) {
    res.dptr[i] = lhs.dptr[i] + rhs.dptr[i];
  }
  return res;
}
/*
这种方式低效，因为在每次只需加操作时，需要创建临时变量。虽然可以实现operation +=，
但这限制了我们可以写的等式
*/

/**
 * Lazy evaluation
 *
 * 为什么上面的operation+需要申请临时变量？
 * 因为在只需operator+的时候，我们不知道要把值赋给谁！如果知道的话，就可以直接把值赋给目标
 */
