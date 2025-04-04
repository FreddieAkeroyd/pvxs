/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvxs is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <testMain.h>

#include <epicsUnitTest.h>

#include <pvxs/unittest.h>
#include <pvxs/nt.h>

namespace {

using namespace pvxs;

void testNTScalar()
{
    testDiag("In %s", __func__);

    // plain, without display meta
    auto top = nt::NTScalar{TypeCode::Int32}.create();

    testTrue(top.idStartsWith("epics:nt/NTScalar:"))<<"\n"<<top;

    testEq(top["value"].type(), TypeCode::Int32);
    testEq(top["display.limitLow"].type(), TypeCode::Null);
    testEq(top["display.description"].type(), TypeCode::Null);
    testEq(top["control.limitLow"].type(), TypeCode::Null);

    // with display, but not control
    top = nt::NTScalar{TypeCode::Float64,true}.create();

    testEq(top["value"].type(), TypeCode::Float64);
    testEq(top["display.limitLow"].type(), TypeCode::Float64);
    testEq(top["display.description"].type(), TypeCode::String);
    testEq(top["control.limitLow"].type(), TypeCode::Null);

    // with everything
    top = nt::NTScalar{TypeCode::Float64,true,true,true}.create();

    testEq(top["value"].type(), TypeCode::Float64);
    testEq(top["display.limitLow"].type(), TypeCode::Float64);
    testEq(top["display.description"].type(), TypeCode::String);
    testEq(top["control.limitLow"].type(), TypeCode::Float64);
}

void testNTNDArray()
{
    testDiag("In %s", __func__);

    auto top = nt::NTNDArray{}.create();

    testTrue(top.idStartsWith("epics:nt/NTNDArray:"))<<"\n"<<top;

    top["value->floatValue"] = shared_array<const float>({0.1, 0.2, 0.3, 0.4, 0.5});
    testEq(top["value"].type(), TypeCode::Union);
    top = top["value"].lookup("->");
    testEq(top.type(), TypeCode::Float32A);

}

void testNTURI()
{
    testDiag("In %s", __func__);

    using namespace pvxs::members;

    auto def = nt::NTURI({
                             UInt32("arg1"),
                             String("arg2"),
                         });

    auto top = def.call(42, "hello");

    testTrue(top.idStartsWith("epics:nt/NTURI:"))<<"\n"<<top;
    testEq(top["query.arg1"].as<uint32_t>(), 42u);
    testEq(top["query.arg2"].as<std::string>(), "hello");
}

void testNTEnum()
{
    testDiag("In %s", __func__);

    auto top = nt::NTEnum{}.create();

    testTrue(top.idStartsWith("epics:nt/NTEnum:"))<<"\n"<<top;
}

void testNTTable()
{
    testDiag("In %s", __func__);

    auto top = nt::NTTable{}
            .add_column(TypeCode::Int32, "A", "Col A")
            .add_column(TypeCode::String, "C", "Col C")
            .add_column(TypeCode::String, "B", "Col B")
            .create();

    shared_array<const std::string> labels({"Col A", "Col C", "Col B"});
    testArrEq(top["labels"].as<shared_array<const std::string>>(), labels);
    testTrue(top["value.A"].type()==TypeCode::Int32A);
    testTrue(top["value.B"].type()==TypeCode::StringA);

    std::vector<std::string> names;
    for(auto fld : top["value"].ichildren()) {
        names.push_back(top.nameOf(fld));
    }
    shared_array<const std::string> expect({"value.A", "value.C", "value.B"});
    testArrEq(shared_array<const std::string>(names.begin(), names.end()), expect);
}

} // namespace

MAIN(testnt) {
    testPlan(24);
    testNTScalar();
    testNTNDArray();
    testNTURI();
    testNTEnum();
    testNTTable();
    return testDone();
}
