#include <gtest/gtest.h>
#include "Tape.hpp"
#include <factory_functions.hpp>
class TapeWriterTest : public ::testing::Test
{
  public:
    TapeWriterTest()
    {
    }
    /*std::vector<Record::SerializedRecord> ReadRecordsFromFile(const std::string_view file_path)
    {
        std::ifstream in(file_path.data(), std::ios::binary);
        if (in)
        {
            Record::memory_vector temp;
            std::copy(temp.begin(), temp.end(), std::istreambuf_iterator(in));
            for (int i = 0; i < factory_funcs::vectorsizeof(temp) / sizeof(Record::SerializedRecord); ++i)
            {
            }
        }
    }*/
  protected:
};

TEST(TapeWriterTest, DISABLED_sunny_scenario_WriteRecordsToFile)
{
    
}

