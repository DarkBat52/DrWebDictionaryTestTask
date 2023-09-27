#include "pch.h"
#include "dictionary.h"

template<class Key, class Value>
using concrete_dictionary_t = map<Key, Value>;

class MapTest : public ::testing::Test
{
public:

	dictionary<int, float>* dic;
	dictionary<int, float>* filled_dic;
	
protected:

    MapTest() {

        //filled out dic
        filled_dic = new concrete_dictionary_t<int, float>();
        filled_dic->set(2, 1.5f);
        filled_dic->set(4, 2.5f);
        filled_dic->set(7, 6.2f);
        filled_dic->set(99, 7.8f);
    }

    ~MapTest() override {
        
    }

    void SetUp() override {
        //empty dic
        dic = new concrete_dictionary_t<int, float>();

    }

    void TearDown() override {
        delete dic;
        delete filled_dic;
    }

};

TEST_F(MapTest, TestIsSet) {
    EXPECT_TRUE(filled_dic->is_set(7)) << "key of value 7 was not set";
}

TEST_F(MapTest, TestIsNotSet) {
    EXPECT_FALSE(filled_dic->is_set(0)) << "key of value 0 was not set but is_set returned true";
}

TEST_F(MapTest, TestGet) {
    EXPECT_EQ(filled_dic->get(2), 1.5f);
    EXPECT_EQ(filled_dic->get(99), 7.8f);
}

//since capacity is not available trhough the dictionary interface, this is the only way to test updated capasity 
TEST_F(MapTest, TestUpdateCapacity) {
    // keep setting values until capacity is updated
    for (int i = 0; i < 13; ++i) {
        dic->set(i * 2 + 3, 1.4 * i);
    }
    //capacity has updated; test if all the keys are set
    for (int i = 0; i < 13; ++i) {
        EXPECT_TRUE(dic->is_set(i * 2 + 3)) << i * 2 + 3 << " key is not set for some reason";
    }
}

//test if not_found_exception is thrown
TEST_F(MapTest, TestNotFoundException) {
    EXPECT_THROW(filled_dic->get(999), not_found_exception<int>);
}
