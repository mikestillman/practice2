// Copyright 2011 Michael E. Stillman

#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <memory>
#include <gtest/gtest.h>
#include <mpfr.h>

#include "buffer.hpp"

bool testfcn() { return true; }

TEST(Nothing, ideal) {
    EXPECT_EQ(true, testfcn());
    EXPECT_FALSE(!(testfcn()));
}

TEST(Buffer, make1) {
    buffer o;
    char *s = o.str();
    //fprintf(stderr, ".[%s].\n", s);
    int c = strcmp("", s);
    EXPECT_EQ(c, 0);
}

TEST(Buffer, make2) {
    buffer o;
    o << "hi there";
    char *s = o.str();
    //fprintf(stderr, "..%s..\n", s);
    int c = strcmp("hi there", s);
    EXPECT_EQ(c, 0);
}

