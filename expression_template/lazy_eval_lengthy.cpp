#include <cstdio>

// Exp that all expression must inheriate it
template<typename SubType>
struct Exp {
    // return const reference of the actual type of this expression
    inline const SubType& self(void) const{
        return *static_cast<const SubType*>(this);
    }
};


// binary add expression
// note how it is inheritates from Exp
// and put its own type into the template argument
template <typename TLhs, typename RLhs>
struct BinaryAddExp : public Exp<BinaryAddExp<TLhs, RLhs> > {
    const TLhs &lhs;
    const RLhs &rhs;
    BinaryAddExp(const TLhs &lhs, const RLhs &rhs) : lhs(lhs), rhs(rhs){}

    // evaluation function, evaluate this expression at position
    inline float Eval(int i) const {
        return lhs.Eval(i) + rhs.Eval(i); // 这个地方不是也要产生一个临时变量吗？
    }
};

struct Vec: public Exp<Vec> {
    int len;
    float *dptr;
    Vec(float *dptr, int len): len(len), dptr(dptr) {}

    // Here is evaluation happens
    template<typename Etype>
    inline Vec& operator=(const Exp<Etype> &src_){
        const Etype &src = src_.self();
        for(int i=0; i<len; i++){
            dptr[i] = src.Eval(i);
        }
        return *this;
    }

    inline float Eval(int i) const{
        return dptr[i];
    }
};

// template add, works for any expressions
template <typename TLhs, typename RLhs>
inline BinaryAddExp<TLhs, RLhs> operator+(const Exp<TLhs> &lhs, const Exp<RLhs> &rhs)
{
    return BinaryAddExp<TLhs, RLhs>(lhs.self(), rhs.self()); // 返回时也有临时空间被申请吧
}

const int n = 3;
int main(void)
{
    float sa[n] = {1, 2, 3};
    float sb[n] = {2, 3, 4};
    float sc[n] = {3, 4, 5};
    Vec A(sa, n), B(sb, n), C(sc, n);
    // run expression, this expression is longer:)
    A = B + C + C;
    for (int i = 0; i < n; ++i) {
        printf("%d:%f == %f + %f + %f\n", i,
            A.dptr[i], B.dptr[i],
            C.dptr[i], C.dptr[i]);
    }
    return 0;

}