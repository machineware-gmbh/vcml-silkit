#ifndef VCML_TESTING_H
#define VCML_TESTING_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <systemc>
#include <mwr.h>
#include <vcml.h>

#include "vcml-silkit.h"

using namespace ::testing;
using namespace ::sc_core;
using namespace ::vcml;

class test_base : public component
{
private:
    tracer_term m_tracer;
    mwr::publishers::terminal m_logger;

    generic::reset m_reset;
    generic::clock m_clock;

    void run();

public:
    test_base() = delete;
    test_base(const sc_module_name& nm);
    virtual ~test_base();

    virtual void run_test() = 0;
    virtual void finalize_test();
};

extern vector<string> args;

string get_resource_path(const string& name);

#endif
