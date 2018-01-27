/**
 * @file    segment_tree.cpp
 * @author  leemonage
 * @date    1/28/2018
 * @brief   segment tree implementation with some examples of usage
 */

#include <functional>
#include <iostream>
#include <vector>

/**
 * @brief  class segment_tree<T> implements the main functions
 *         of simple segment tree -- update value in point and calculate
 *         some function on segment [left, right].
 */

template<class T> class segment_tree {
private:
    size_t n;                     /* number of elements */
    std::vector<T> tree;          /* information in vertices */
    std::function<T(T, T)> foo;   /* function (by default sum of numbers) */

    /* builds segment tree by given vector of elements */
    
    void build(const std::vector<T>& vec, int v, size_t l, size_t r) {
        if (l == r) {
            tree[v] = vec[l];
        } else {
            auto mid = (l + r) / 2;
            build(vec, 2 * v, l, mid);
            build(vec, 2 * v + 1, mid + 1, r);
            tree[v] = foo(tree[2 * v], tree[2 * v + 1]);
        }
    }

    /* updates value in point */
    
    void update(const size_t& idx, const T& val, int v, size_t l, size_t r) {
        if (l == r) {
            tree[v] = val;
        } else {
            auto mid = (l + r) / 2;
            
            if (idx <= mid)
                update(idx, val, 2 * v, l, mid);
            else
                update(idx, val, 2 * v + 1, mid + 1, r);
                
            tree[v] = foo(tree[2 * v], tree[2 * v + 1]);
        }
    }

    /* calculates function foo on segment [left, right] */
    
    T get(const size_t& left, const size_t& right, int v, size_t l, size_t r) {
        if (left <= l && r <= right)
            return tree[v];
            
        auto mid = (l + r) / 2;
        
        if (right <= mid)
            return get(left, right, 2 * v, l, mid);
            
        if (left > mid)
            return get(left, right, 2 * v + 1, mid + 1, r);
            
        return foo(
                get(left, right, 2 * v, l, mid),
                get(left, right, 2 * v + 1, mid + 1, r)
            );
    }
    
public:

    /* default constructor */

    segment_tree() {
        n = 0;
        tree.resize(0);
        foo = [](T a, T b) { return a + b; }
    }

    /* constructor by number of elements and function */

    segment_tree(int n, const std::function<T(T, T)> foo) : n(n), foo(foo) {
        tree.assign(4 * n, T(0));
    }

    /* constructor by number of elements */
    
    segment_tree(int n) : n(n) {
        tree.assign(4 * n, T(0));
        foo = [](T a, T b) { return a + b; };
    }

    /* constructor by vector of elements and function */
    
    segment_tree(const std::vector<T>& vec, const std::function<T(T, T)> foo) : foo(foo) {
        n = vec.size();
        tree.assign(4 * n, T(0));
        
        if (n != 0)
            build(vec, 1, 0, n - 1);
    }

    /* constructor by vector of elements */
    
    segment_tree(const std::vector<T>& vec) {
        n = vec.size();
        tree.assign(4 * n, T(0));
        foo = [](T a, T b) { return a + b; };
        
        if (n != 0)
            build(vec, 1, 0, n - 1);
    }
    
    void update_point(size_t point, T new_val) {
        if (point < n)
            update(point, new_val, 1, 0, n - 1);
    }
    
    T get(size_t left, size_t right) {
        if (left <= right && right < n)
            return get(left, right, 1, 0, n - 1);
        else
            return T(0);
    }
};

/* usage */

int main() {
    std::vector<int> v = {2, 4, 1, 42, 9};
    
    segment_tree<int> st_sum(v);
    
    std::cout << st_sum.get(1, 3) << std::endl;     // 47
    st_sum.update_point(2, 7);
    std::cout << st_sum.get(1, 3) << std::endl;     // 53
    std::cout << st_sum.get(2, 3) << std::endl;     // 43
    
    segment_tree<int> st_prd(v, [](int a, int b) { return a * b; });
    
    std::cout << st_prd.get(1, 3) << std::endl;     // 168
    st_prd.update_point(2, 7);
    std::cout << st_prd.get(1, 3) << std::endl;     // 1176
    std::cout << st_prd.get(2, 3) << std::endl;     // 294

    return 0;
}

