#include "element.h"
#include "socow-vector.h"

#include <gtest/gtest.h>

template class socow_vector<int, 2>;
template class socow_vector<element, 2>;
template class socow_vector<std::string, 2>;

using std::as_const;

using container = socow_vector<element, 2>;

TEST(correctness, default_ctor) {
  container a;
  element::expect_no_instances();
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(0, a.size());
}

TEST(correctness, push_back) {
  const size_t N = 5000;
  {
    container a;
    for (size_t i = 0; i != N; ++i) {
      a.push_back(i);
    }

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(i, a[i]);
    }
  }

  element::expect_no_instances();
}

TEST(correctness, push_back_from_self) {
  const size_t N = 500;
  {
    container a;
    a.push_back(42);
    for (size_t i = 0; i != N; ++i) {
      a.push_back(a[0]);
    }

    for (size_t i = 0; i != a.size(); ++i) {
      EXPECT_EQ(42, a[i]);
    }
  }

  element::expect_no_instances();
}

TEST(correctness, subscription) {
  const size_t N = 500;
  socow_vector<size_t, 2> a;
  for (size_t i = 0; i != N; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = 0; i != N; ++i) {
    EXPECT_EQ(2 * i + 1, a[i]);
  }

  const socow_vector<size_t, 2>& ca = a;

  for (size_t i = 0; i != N; ++i) {
    EXPECT_EQ(2 * i + 1, ca[i]);
  }
}

TEST(correctness, subscription_2) {
  container v;
  v.push_back(3);
  v.push_back(7);

  EXPECT_EQ(3, as_const(v)[0]);
  EXPECT_EQ(7, as_const(v)[1]);
}

TEST(correctness, subscription_3) {
  container v;
  v.push_back(3);
  v.push_back(7);

  v[0] = 4;

  EXPECT_EQ(4, v[0]);
  EXPECT_EQ(7, v[1]);
}

TEST(correctness, data) {
  const size_t N = 500;
  container a;

  for (size_t i = 0; i != N; ++i) {
    a.push_back(2 * i + 1);
  }

  {
    element* ptr = a.data();
    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * i + 1, ptr[i]);
    }
  }

  {
    const element* cptr = as_const(a).data();
    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * i + 1, cptr[i]);
    }
  }
}

TEST(correctness, data_2) {
  container v;
  v.push_back(3);
  v.push_back(7);

  element* data = v.data();
  EXPECT_EQ(3, data[0]);
  EXPECT_EQ(7, data[1]);
}

TEST(correctness, data_3) {
  container v;
  v.push_back(3);
  v.push_back(7);

  const element* data = as_const(v).data();
  EXPECT_EQ(3, data[0]);
  EXPECT_EQ(7, data[1]);
}

TEST(correctness, front_back) {
  const size_t N = 500;
  container a;
  for (size_t i = 0; i != N; ++i) {
    a.push_back(2 * i + 1);
  }

  EXPECT_EQ(1, a.front());
  EXPECT_EQ(1, as_const(a).front());

  EXPECT_EQ(999, a.back());
  EXPECT_EQ(999, as_const(a).back());
}

TEST(correctness, front_back_2) {
  container a;
  a.push_back(13);
  a.push_back(17);

  EXPECT_EQ(13, a.front());
  EXPECT_EQ(13, as_const(a).front());

  EXPECT_EQ(17, a.back());
  EXPECT_EQ(17, as_const(a).back());
}

TEST(correctness, capacity) {
  const size_t N = 500;
  {
    container a;
    a.reserve(N);
    EXPECT_LE(N, a.capacity());
    for (size_t i = 0; i != N - 1; ++i) {
      a.push_back(2 * i + 1);
    }
    EXPECT_LE(N, a.capacity());
    a.shrink_to_fit();
    EXPECT_EQ(N - 1, a.capacity());
  }
  element::expect_no_instances();
}

TEST(correctness, capacity_2) {
  socow_vector<element, 3> a;
  EXPECT_EQ(3, a.capacity());
}

TEST(correctness, capacity_3) {
  socow_vector<element, 3> a;
  a.reserve(2);
  EXPECT_EQ(3, a.capacity());
}

TEST(correctness, reserve) {
  container a;
  a.reserve(10);
  for (size_t i = 0; i != 3; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  b.reserve(5);
  uintptr_t old_data = reinterpret_cast<uintptr_t>(as_const(b).data());
  for (size_t i = 3; i != 5; ++i) {
    b.push_back(i + 100);
  }
  EXPECT_EQ(old_data, reinterpret_cast<uintptr_t>(as_const(b).data()));
}

TEST(correctness, reserve_2) {
  {
    socow_vector<element, 3> a;
    a.reserve(10);
    for (size_t i = 0; i != 5; ++i) {
      a.push_back(i + 100);
    }

    socow_vector<element, 3> b = a;
    b.reserve(3);
  }
  element::expect_no_instances();
}

TEST(correctness, superfluous_reserve) {
  const size_t N = 500, K = 100;
  {
    container a;
    a.reserve(N);
    EXPECT_GE(a.capacity(), N);
    a.reserve(K);
    EXPECT_GE(a.capacity(), N);
  }
  element::expect_no_instances();
}

TEST(correctness, clear) {
  const size_t N = 500;

  container a;
  for (size_t i = 0; i != N; ++i) {
    a.push_back(2 * i + 1);
  }
  size_t c = a.capacity();
  a.clear();
  EXPECT_EQ(c, a.capacity());

  element::expect_no_instances();
}

TEST(correctness, clear_2) {
  socow_vector<element, 3> a;

  for (size_t i = 0; i != 2; ++i) {
    a.push_back(i + 100);
  }
  size_t c = a.capacity();
  a.clear();
  EXPECT_EQ(c, a.capacity());
  EXPECT_EQ(0, a.size());
  EXPECT_TRUE(a.empty());

  element::expect_no_instances();
}

TEST(correctness, superfluous_shrink_to_fit) {
  const size_t N = 500;
  {
    container a;
    a.reserve(N);
    size_t c = a.capacity();
    for (size_t i = 0; i != c; ++i) {
      a.push_back(2 * i + 1);
    }
    element* old_data = a.data();
    a.shrink_to_fit();
    EXPECT_EQ(old_data, a.data());
  }
  element::expect_no_instances();
}

TEST(correctness, shrink_to_fit) {
  const size_t N = 10;
  container a;
  for (size_t i = 0; i != N; ++i) {
    a.push_back(i);
  }
  a.clear();
  a.shrink_to_fit();
  a.push_back(1);
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(1, a.size());
}

TEST(correctness, shrink_to_fit_2) {
  socow_vector<element, 2> a;
  a.push_back(123);
  a.shrink_to_fit();
  EXPECT_EQ(1, a.size());
  EXPECT_EQ(2, a.capacity());
}

TEST(correctness, copy_ctor) {
  const size_t N = 500;
  {
    container a;
    for (size_t i = 0; i != N; ++i) {
      a.push_back(i);
    }

    container b = a;
    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(i, b[i]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, assignment_operator) {
  const size_t N = 500;
  {
    container a;
    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }

    container b;
    b.push_back(42);

    b = a;
    EXPECT_EQ(N, b.size());
    for (size_t i = 0; i != N; ++i) {
      auto tmp = b[i];
      EXPECT_EQ(2 * i + 1, tmp);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, self_assignment) {
  const size_t N = 500;
  {
    container a;
    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }
    a = a;

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * i + 1, a[i]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, pop_back) {
  const size_t N = 500;
  container a;

  for (size_t i = 0; i != N; ++i) {
    a.push_back(2 * i + 1);
  }

  for (size_t i = N; i != 0; --i) {
    EXPECT_EQ(2 * i - 1, a.back());
    EXPECT_EQ(i, a.size());
    a.pop_back();
  }
  EXPECT_TRUE(a.empty());
  element::expect_no_instances();
}

TEST(correctness, pop_back_2) {
  container a;

  a.push_back(41);
  a.push_back(43);

  a.pop_back();
  EXPECT_EQ(1, a.size());
  EXPECT_EQ(41, a[0]);
  a.pop_back();
  EXPECT_EQ(0, a.size());
}

TEST(correctness, pop_back_3) {
  socow_vector<element, 3> a;
  a.push_back(41);
  a.push_back(43);
  a.push_back(47);
  a.push_back(51);
  a.pop_back();

  socow_vector<element, 3> b = a;
  try {
    element::set_throw_countdown(2);
    a.pop_back();
  } catch (const std::runtime_error&) {
    EXPECT_EQ(3, a.size());
    EXPECT_EQ(41, a[0]);
    EXPECT_EQ(43, a[1]);
    EXPECT_EQ(47, a[2]);
    return;
  }

  element::set_throw_countdown(0);

  EXPECT_EQ(2, a.size());
  EXPECT_EQ(41, a[0]);
  EXPECT_EQ(43, a[1]);
}

TEST(correctness, insert_begin) {
  const size_t N = 500;
  container a;

  for (size_t i = 0; i != N; ++i) {
    a.insert(a.begin(), i);
  }

  for (size_t i = 0; i != N; ++i) {
    EXPECT_EQ(i, a.back());
    a.pop_back();
  }
  element::expect_no_instances();
}

TEST(correctness, insert_end) {
  const size_t N = 500;
  {
    container a;

    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }
    EXPECT_EQ(N, a.size());

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(N + i, a.size());
      a.insert(a.end(), 4 * i + 1);
      EXPECT_EQ(4 * i + 1, a.back());
    }

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * i + 1, a[i]);
    }
  }
  element::expect_no_instances();
}

TEST(performance, insert) {
  const size_t N = 10000;
  socow_vector<socow_vector<size_t, 2>, 2> a;

  for (size_t i = 0; i < N; ++i) {
    a.push_back(socow_vector<size_t, 2>());
    for (size_t j = 0; j < N; ++j) {
      a.back().push_back(j);
    }
  }

  socow_vector<size_t, 2> temp;
  for (size_t i = 0; i < N; ++i) {
    temp.push_back(i);
  }
  a.insert(a.begin(), temp);
}

TEST(correctness, insert_empty) {
  container v;
  const size_t N = 5;
  for (size_t i = 0; i != N; ++i) {
    v.push_back(42);
  }

  for (size_t i = 0; i != N; ++i) {
    v.pop_back();
  }

  v.insert(v.begin(), 43);
  EXPECT_EQ(1, v.size());
  EXPECT_EQ(43, v[0]);
}

TEST(correctness, erase) {
  const size_t N = 500;
  {
    for (size_t i = 0; i != N; ++i) {
      container a;
      for (size_t j = 0; j != N; ++j) {
        a.push_back(2 * j + 1);
      }

      a.erase(a.begin() + i);
      size_t cnt = 0;
      for (size_t j = 0; j != N - 1; ++j) {
        if (j == i) {
          ++cnt;
        }
        EXPECT_EQ(2 * cnt + 1, a[j]);
        ++cnt;
      }
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_begin) {
  const size_t N = 500;
  {
    container a;

    for (size_t i = 0; i != 2 * N; ++i) {
      a.push_back(2 * i + 1);
    }

    for (size_t i = 0; i != N; ++i) {
      a.erase(a.begin());
    }

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * (i + N) + 1, a[i]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_end) {
  const size_t N = 500;
  {
    container a;

    for (size_t i = 0; i != 2 * N; ++i) {
      a.push_back(2 * i + 1);
    }

    for (size_t i = 0; i != N; ++i) {
      a.erase(a.end() - 1);
    }

    for (size_t i = 0; i != N; ++i) {
      EXPECT_EQ(2 * i + 1, a[i]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_range_begin) {
  const size_t N = 500, K = 100;
  {
    container a;

    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }

    a.erase(a.begin(), a.begin() + K);

    for (size_t i = 0; i != N - K; ++i) {
      EXPECT_EQ(2 * (i + K) + 1, a[i]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_range_middle) {
  const size_t N = 500, K = 100;
  {
    container a;

    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }

    a.erase(a.begin() + K, a.end() - K);

    for (size_t i = 0; i != K; ++i) {
      EXPECT_EQ(2 * i + 1, a[i]);
    }
    for (size_t i = 0; i != K; ++i) {
      EXPECT_EQ(2 * (i + N - K) + 1, a[i + K]);
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_range_end) {
  const size_t N = 500, K = 100;
  {
    container a;

    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }

    a.erase(a.end() - K, a.end());
    for (size_t i = 0; i != N - K; ++i) {
      EXPECT_EQ(2 * i + 1, a[i]);
    }
  }

  element::expect_no_instances();
}

TEST(correctness, erase_range_all) {
  const size_t N = 500;
  {
    container a;

    for (size_t i = 0; i != N; ++i) {
      a.push_back(2 * i + 1);
    }

    a.erase(a.begin(), a.end());

    EXPECT_TRUE(a.empty());
  }

  element::expect_no_instances();
}

TEST(correctness, erase_big_range) {
  {
    container c;
    for (size_t i = 0; i != 100; ++i) {
      for (size_t j = 0; j != 5000; ++j) {
        c.push_back(j);
      }
      c.erase(c.begin() + 100, c.end() - 100);
      c.clear();
    }
  }
  element::expect_no_instances();
}

TEST(correctness, erase_1) {
  container v;
  v.push_back(100);
  v.push_back(101);

  v.erase(v.begin());
  EXPECT_EQ(1, v.size());
  EXPECT_EQ(101, v[0]);
}

TEST(correctness, erase_2) {
  container v;
  v.push_back(100);
  v.push_back(101);

  v.erase(v.begin() + 1);
  EXPECT_EQ(1, v.size());
  EXPECT_EQ(100, v[0]);
}

TEST(correctness, erase_3) {
  container v;
  v.push_back(100);
  v.push_back(101);

  v.erase(v.begin(), v.begin());
  EXPECT_EQ(2, v.size());
  EXPECT_EQ(100, v[0]);
  EXPECT_EQ(101, v[1]);
}

TEST(correctness, erase_4) {
  container v;
  v.push_back(100);
  v.push_back(101);

  v.erase(v.begin(), v.end());
  EXPECT_TRUE(v.empty());
}

TEST(correctness, reallocation_throw) {
  {
    container a;
    a.reserve(10);
    size_t n = a.capacity();
    for (size_t i = 0; i != n; ++i) {
      a.push_back(i);
    }
    element::set_throw_countdown(7);
    EXPECT_THROW(a.push_back(42), std::runtime_error);
  }
  element::expect_no_instances();
}

// This test actually checks memory leak in pair with @valgrind
TEST(correctness, copy_throw) {
  container a;
  a.reserve(10);
  size_t n = a.capacity();
  for (size_t i = 0; i != n; ++i) {
    a.push_back(i);
  }
  element::set_throw_countdown(7);
  EXPECT_NO_THROW({ container b(a); });
  element::set_throw_countdown(0);
}

TEST(correctness, iter_types) {
  using el_t = element;
  using vec_t = socow_vector<el_t, 2>;
  bool test1 = std::is_same<el_t*, typename vec_t::iterator>::value;
  bool test2 = std::is_same<const el_t*, typename vec_t::const_iterator>::value;
  EXPECT_TRUE(test1);
  EXPECT_TRUE(test2);
}

TEST(correctness_cow, copy_ctor) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, subscript) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  b[3] = 42;
  EXPECT_EQ(103, a[3]);
}

TEST(correctness_cow, subscript_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  a[3] = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, subscript_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b)[3];
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, data) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  b.data()[3] = 42;
  EXPECT_EQ(103, a[3]);
}

TEST(correctness_cow, data_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  a.data()[3] = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, data_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  EXPECT_EQ(103, as_const(b).data()[3]);
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, front) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  b.front() = 42;
  EXPECT_EQ(100, a.front());
}

TEST(correctness_cow, front_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  a.front() = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, front_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).front();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, back) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  b.back() = 42;
  EXPECT_EQ(103, a.back());
}

TEST(correctness_cow, back_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  a.back() = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, back_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).back();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, push_back) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  a.push_back(1);
  b.push_back(2);

  EXPECT_EQ(1, a[4]);
  EXPECT_EQ(2, b[4]);
}

TEST(correctness_cow, pop_back) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  a.pop_back();

  EXPECT_EQ(3, a.size());
  EXPECT_EQ(4, b.size());

  element t = b[3];
  EXPECT_EQ(103, t);
}

TEST(correctness_cow, reserve) {
  container a;
  a.reserve(5);
  a.push_back(1);

  container b = a;
  b.reserve(5);

  EXPECT_NE(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, shrink_to_fit) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  a.shrink_to_fit();

  EXPECT_NE(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, shrink_to_fit_empty) {
  container a;
  a.reserve(5);

  container b = a;

  a.shrink_to_fit();

  EXPECT_NE(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, clear) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;

  a.clear();

  for (size_t i = 0; i != 4; ++i) {
    EXPECT_EQ(i + 100, b[i]);
  }
}

TEST(correctness_cow, begin) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  *b.begin() = 42;
  EXPECT_EQ(100, *a.begin());
}

TEST(correctness_cow, begin_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  *a.begin() = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, begin_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).begin();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, end) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  *std::prev(b.end()) = 42;
  EXPECT_EQ(103, *std::prev(a.end()));
}

TEST(correctness_cow, end_single_user) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  element::reset_copy_counter();
  *std::prev(a.end()) = 42;
  element::expect_copy_counter(1);
}

TEST(correctness_cow, end_const) {
  container a;
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  container b = a;
  as_const(b).end();
  EXPECT_EQ(as_const(a).data(), as_const(b).data());
}

TEST(correctness_cow, insert) {
  container a;
  a.reserve(5);
  a.push_back(100);
  a.push_back(101);
  a.push_back(103);
  a.push_back(104);

  container b = a;
  a.insert(as_const(a).begin() + 2, 102);

  for (size_t i = 0; i != 5; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
  }

  EXPECT_EQ(100, b[0]);
  EXPECT_EQ(101, b[1]);
  EXPECT_EQ(103, b[2]);
  EXPECT_EQ(104, b[3]);
}

TEST(correctness_cow, insert_single_user) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i != 4; ++i) {
    a.push_back(i + 100);
  }

  uintptr_t old_data = reinterpret_cast<uintptr_t>(as_const(a).data());
  a.insert(as_const(a).begin() + 2, 200);
  EXPECT_EQ(old_data, reinterpret_cast<uintptr_t>(as_const(a).data()));
}

TEST(correctness_cow, erase) {
  container a;
  a.reserve(5);
  a.push_back(100);
  a.push_back(101);
  a.push_back(200);
  a.push_back(102);
  a.push_back(103);

  container b = a;
  a.erase(as_const(a).begin() + 2);

  for (size_t i = 0; i != 4; ++i) {
    EXPECT_EQ(i + 100, as_const(a)[i]);
  }

  EXPECT_EQ(100, b[0]);
  EXPECT_EQ(101, b[1]);
  EXPECT_EQ(200, b[2]);
  EXPECT_EQ(102, b[3]);
  EXPECT_EQ(103, b[4]);
}

TEST(correctness_cow, erase_single_user) {
  container a;
  a.reserve(5);
  for (size_t i = 0; i != 5; ++i) {
    a.push_back(i + 100);
  }

  uintptr_t old_data = reinterpret_cast<uintptr_t>(as_const(a).data());
  a.erase(as_const(a).begin() + 2);
  EXPECT_EQ(old_data, reinterpret_cast<uintptr_t>(as_const(a).data()));
}

TEST(small_object, shrink_to_fit) {
  socow_vector<element, 3> a;
  a.reserve(5);
  for (size_t i = 0; i != 5; ++i) {
    a.push_back(i + 100);
  }
  for (size_t i = 0; i != 3; ++i) {
    a.pop_back();
  }

  EXPECT_LE(5, a.capacity());
  a.shrink_to_fit();
  EXPECT_EQ(3, a.capacity());
}

TEST(small_object, shrink_to_fit_2) {
  socow_vector<element, 3> a;
  a.reserve(5);
  for (size_t i = 0; i != 5; ++i) {
    a.push_back(i + 100);
  }
  for (size_t i = 0; i != 3; ++i) {
    a.pop_back();
  }

  EXPECT_LE(5, a.capacity());
  element::set_throw_countdown(2);
  EXPECT_THROW(a.shrink_to_fit(), std::runtime_error);
  EXPECT_EQ(5, a.capacity());
  EXPECT_EQ(2, a.size());
  EXPECT_EQ(100, a[0]);
  EXPECT_EQ(101, a[1]);
}

TEST(small_object, swap_two_small) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);

  socow_vector<element, 3> b;
  b.push_back(3);

  a.swap(b);

  EXPECT_EQ(1, a.size());
  EXPECT_EQ(2, b.size());
  EXPECT_EQ(1, b[0]);
  EXPECT_EQ(2, b[1]);
  EXPECT_EQ(3, a[0]);

  a.swap(b);

  EXPECT_EQ(1, b.size());
  EXPECT_EQ(2, a.size());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, b[0]);
}

TEST(small_object, swap_big_and_small) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  socow_vector<element, 3> b;
  b.push_back(5);

  a.swap(b);

  EXPECT_EQ(1, a.size());
  EXPECT_EQ(4, b.size());
  EXPECT_EQ(1, b[0]);
  EXPECT_EQ(2, b[1]);
  EXPECT_EQ(3, b[2]);
  EXPECT_EQ(4, b[3]);
  EXPECT_EQ(5, a[0]);

  a.swap(b);

  EXPECT_EQ(1, b.size());
  EXPECT_EQ(4, a.size());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, a[2]);
  EXPECT_EQ(4, a[3]);
  EXPECT_EQ(5, b[0]);
}

TEST(small_object, swap_big_and_small_2) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  socow_vector<element, 3> b;
  b.push_back(5);
  b.push_back(6);

  element::set_throw_countdown(2);
  EXPECT_THROW(a.swap(b), std::runtime_error);

  EXPECT_EQ(4, a.size());
  EXPECT_EQ(2, b.size());
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(3, a[2]);
  EXPECT_EQ(4, a[3]);
  EXPECT_EQ(5, b[0]);
  EXPECT_EQ(6, b[1]);
}

TEST(small_object, swap_two_big) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);

  socow_vector<element, 3> b;
  b.push_back(5);
  b.push_back(6);
  b.push_back(7);
  b.push_back(8);
  b.push_back(9);

  a.swap(b);

  EXPECT_EQ(5, a.size());
  EXPECT_EQ(4, b.size());
  EXPECT_EQ(6, a[1]);
  EXPECT_EQ(3, b[2]);

  a.swap(b);

  EXPECT_EQ(4, a.size());
  EXPECT_EQ(5, b.size());
  EXPECT_EQ(2, a[1]);
  EXPECT_EQ(7, b[2]);
}

TEST(small_object, begin_end) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  auto it = a.begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(2, *it);
  ++it;
  EXPECT_TRUE(it == a.end());
}

TEST(small_object, begin_end_const) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  auto it = as_const(a).begin();
  EXPECT_EQ(1, *it);
  ++it;
  EXPECT_EQ(2, *it);
  ++it;
  EXPECT_TRUE(it == as_const(a).end());
}

TEST(small_object, big_empty_range) {
  socow_vector<element, 3> a;
  a.push_back(1);
  a.push_back(2);
  a.push_back(3);
  a.push_back(4);
  a.push_back(5);

  auto it = a.begin() + 3;
  a.erase(it, it);

  EXPECT_EQ(5, a.size());
}

TEST(small_object, erase_big_into_small) {
  socow_vector<element, 3> a;
  for (size_t i = 0; i != 5; ++i) {
    a.push_back(i + 100);
  }

  socow_vector<element, 3> b = a;

  auto it = a.erase(as_const(a).begin() + 1, as_const(a).end() - 1);
  EXPECT_TRUE(it == as_const(a).begin() + 1);
}

TEST(small_object, erase_big_into_small_2) {
  socow_vector<element, 3> a;
  for (size_t i = 0; i != 6; ++i) {
    a.push_back(i + 100);
  }

  socow_vector<element, 3> b = a;

  element::set_throw_countdown(2);
  EXPECT_THROW(a.erase(as_const(a).begin() + 2, as_const(a).end() - 1), std::runtime_error);
}

TEST(small_object, erase_big_into_small_3) {
  socow_vector<element, 3> a;
  for (size_t i = 0; i != 6; ++i) {
    a.push_back(i + 100);
  }

  socow_vector<element, 3> b = a;

  element::set_throw_countdown(3);
  EXPECT_THROW(a.erase(as_const(a).begin() + 2, as_const(a).end() - 1), std::runtime_error);
}
