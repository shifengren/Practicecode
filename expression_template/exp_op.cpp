#include <cstdio>

template <typename SubType>
struct Exp {
    inline const SubType& self(void) const{
        return *static_cast<const SubType*>(this);
    }
};

// binary operation
struct mul{
    inline static float Map(float a, float b){
        return a*b;
    }
};


// binary operation
// note how it is inherited from Exp
// and put its own type into template argument

template <typename OP, typename TLhs, typename RLhs>
struct BinaryMapExp : public Exp<BinaryMapExp<OP, TLhs, RLhs> >
{
    const TLhs& lhs;
    const RLhs& rhs;
    BinaryMapExp(const TLhs& lhs, const RLhs &rhs): lhs(lhs), rhs(rhs){}
    // evaluation function,
    inline float Eval(int i) const {
        return OP::Map(lhs.Eval(i), rhs.Eval(i));
    }
};

struct Vec : public Exp<Vec> {
    int len;
    float *dptr;
    Vec(float *dptr, int len): dptr(dptr), len(len){}

    template <typename Etype>
    inline Vec& operator=(const Exp<Etype> &src_){
        const Etype &src = src_.self();
        for(int i=0; i<len; ++i){
            dptr[i] = src.Eval(i);
        }
        return *this;
    }

    inline float Eval(int i) const {
        return dptr[i];
    }
};

// template binary operation,works for any expressions
template<typename OP, typename TLhs, typename RLhs>
inline BinaryMapExp<OP, TLhs, RLhs> F(const Exp<TLhs> &lhs, const Exp<RLhs> &rhs)
{
    return BinaryMapExp<OP, TLhs, RLhs>(lhs.self(), rhs.self());
}

//
template <typename TLhs, typename RLhs>
inline BinaryMapExp<mul, TLhs, RLhs> operator*(const Exp<TLhs>& lhs, const Exp<RLhs> &rhs){
    return F<mul>(lhs, rhs);
}

// user defined operator
struct maximum {
    inline static float Map(float a, float b){
        printf("a=%f, b=%f\n", a, b);
        return a>b ? a : b;
    }
};

int main(void)
{
    const int n = 3;
    float sa[n] = {1, 2, 3};
    float sb[n] = {2, 3, 4};
    float sc[n] = {1, 2, 5};
    Vec A(sa, n), B(sb, n), C(sc, n);
    // run expression, this expression is longer:)
    // A = B * F<maximum>(C, B);
    // for (int i = 0; i < n; ++i) {
    //     printf("%d:%f == %f * max(%f, %f)\n",
    //         i, A.dptr[i], B.dptr[i], C.dptr[i], B.dptr[i]);
    // }
    float a[n];
    Vec X(a, n);
    X = F<maximum>(B, C);
    for (int i=0; i<n; ++i){
        printf("%d:%f==max(%f, %f)\n", i, X.dptr[i], B.dptr[i], C.dptr[i]);
    }
    return 0;

}