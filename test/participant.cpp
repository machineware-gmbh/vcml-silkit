#include "testing.h"

class test_bench : public test_base
{
public:
    vcml::silkit::participant model;

    test_bench(const sc_module_name& nm): test_base(nm), model("model") {
        // nothing to do
    }

    virtual void run_test() override {
        // nothing to do
    }
};

TEST(participant, simulate) {
    test_bench bench("system");
    sc_core::sc_start();
}
