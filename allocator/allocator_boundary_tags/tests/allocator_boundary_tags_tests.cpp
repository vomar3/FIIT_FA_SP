#include <gtest/gtest.h>
#include <allocator_dbg_helper.h>
#include <allocator_boundary_tags.h>
#include <client_logger_builder.h>
#include <memory>
#include <list>

logger *create_logger(
    std::vector<std::pair<std::string, logger::severity>> const &output_file_streams_setup,
    bool use_console_stream = true,
    logger::severity console_stream_severity = logger::severity::debug)
{
    std::unique_ptr<logger_builder> logger_builder_instance(new client_logger_builder);
    
    if (use_console_stream)
    {
        logger_builder_instance->add_console_stream(console_stream_severity);
    }
    
    for (auto &output_file_stream_setup: output_file_streams_setup)
    {
        logger_builder_instance->add_file_stream(output_file_stream_setup.first, output_file_stream_setup.second);
    }
    
    logger *logger_instance = logger_builder_instance->build();
    
    return logger_instance;
}

//TODO: recalculate size

TEST(positiveTests, test1)
{
    std::unique_ptr<logger> logger(create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_positive_test_plain_usage.txt",
                logger::severity::information
            }
        }));
    std::unique_ptr<smart_mem_resource> subject(new allocator_boundary_tags(sizeof(int) * 70, nullptr, logger.get(), allocator_with_fit_mode::fit_mode::first_fit));
    
    auto *first_block = reinterpret_cast<int *>(subject->allocate(sizeof(int) * 10));
    auto *second_block = reinterpret_cast<int *>(subject->allocate(sizeof(int) * 10));
    auto *third_block = reinterpret_cast<int *>(subject->allocate(sizeof(int) * 10));
    
    ASSERT_EQ(reinterpret_cast<int*>(reinterpret_cast<char*>(first_block + 10) + sizeof(size_t) + sizeof(void*) * 3), second_block);
    ASSERT_EQ(reinterpret_cast<int*>(reinterpret_cast<char*>(second_block + 10) + sizeof(size_t) + sizeof(void*) * 3), third_block);
    
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(second_block)), 1);
    
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(subject.get());
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);
    auto  *fourth_block = reinterpret_cast<int *>(subject->allocate(sizeof(int) * 1));
    the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
    auto *fifth_block = reinterpret_cast<int *>(subject->allocate(sizeof(int) * 1));
    
    ASSERT_EQ(reinterpret_cast<int*>(reinterpret_cast<char*>(first_block + 10) + sizeof(size_t) + sizeof(void*) * 3), fourth_block);
    ASSERT_EQ(reinterpret_cast<int*>(reinterpret_cast<char*>(fourth_block + 1) + sizeof(size_t) + sizeof(void*) * 3), fifth_block);
    
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(first_block)), 1);
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(third_block)), 1);
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(fourth_block)), 1);
    subject->deallocate(const_cast<void *>(reinterpret_cast<void const *>(fifth_block)), 1);

}

TEST(positiveTests, test2)
{
    std::unique_ptr<logger> logger_instance(create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_false_positive_test_1.txt",
                logger::severity::information
            }
        }));
    std::unique_ptr<smart_mem_resource> allocator_instance(new allocator_boundary_tags(sizeof(unsigned char) * 3000, nullptr, logger_instance.get(), allocator_with_fit_mode::fit_mode::first_fit));
    
    char *first_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char) * 1000));
    char *second_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char) * 0));
    allocator_instance->deallocate(first_block, 1);
    first_block = reinterpret_cast<char *>(allocator_instance->allocate(sizeof(char) * 999));
    auto actual_blocks_state = dynamic_cast<allocator_test_utils *>(allocator_instance.get())->get_blocks_info();
    std::vector<allocator_test_utils::block_info> expected_blocks_state
        {
            { .block_size = 1000 + sizeof(allocator_dbg_helper::block_size_t) + sizeof(allocator_dbg_helper::block_pointer_t) * 3, .is_block_occupied = true },
            { .block_size = sizeof(allocator_dbg_helper::block_size_t) + sizeof(allocator_dbg_helper::block_pointer_t) * 3, .is_block_occupied = true },
            { .block_size = 3000 - (1000 + (sizeof(allocator_dbg_helper::block_size_t) + sizeof(allocator_dbg_helper::block_pointer_t) * 3) * 2), .is_block_occupied = false }
        };
    
    ASSERT_EQ(actual_blocks_state.size(), expected_blocks_state.size());
    for (int i = 0; i < actual_blocks_state.size(); i++)
    {
        ASSERT_EQ(actual_blocks_state[i], expected_blocks_state[i]);
    }
    
    allocator_instance->deallocate(first_block, 1);
    allocator_instance->deallocate(second_block, 1);
}

TEST(falsePositiveTests, test1)
{
    std::unique_ptr<logger> logger_instance(create_logger(std::vector<std::pair<std::string, logger::severity>>
        {
            {
                "allocator_boundary_tags_tests_logs_false_positive_test_2.txt",
                logger::severity::information
            }
        }));
    std::unique_ptr<smart_mem_resource> allocator_instance(new allocator_boundary_tags(3000, nullptr, logger_instance.get(), allocator_with_fit_mode::fit_mode::first_fit));
    
    ASSERT_THROW(static_cast<void>(allocator_instance->allocate(sizeof(char)*  3000)), std::bad_alloc);

}

TEST(own, test1)
{
    std::unique_ptr<logger> logger_instance(create_logger(std::vector<std::pair<std::string, logger::severity>>
                                                    {
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::information
                                                            },
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::debug
                                                            },
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::trace
                                                            },
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::warning
                                                            },
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::error
                                                            },
                                                            {
                                                                    "a.txt",
                                                                    logger::severity::critical
                                                            }
                                                    }));

    std::unique_ptr<smart_mem_resource> alloc(new allocator_boundary_tags(4000, nullptr, logger_instance.get(), allocator_with_fit_mode::fit_mode::first_fit));

    auto first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int)*  250));
    auto second_block = reinterpret_cast<char *>(alloc->allocate(sizeof(char) * 500));
    auto third_block = reinterpret_cast<double *>(alloc->allocate(sizeof(double *) * 250));
    alloc->deallocate(first_block, 1);
    first_block = reinterpret_cast<int *>(alloc->allocate(sizeof(int) * 245));

    std::unique_ptr<smart_mem_resource> allocator(new allocator_boundary_tags(5000, nullptr, logger_instance.get(), allocator_with_fit_mode::fit_mode::first_fit));
    auto *the_same_subject = dynamic_cast<allocator_with_fit_mode *>(allocator.get());
    int iterations_count = 100;

    std::list<void *> allocated_blocks;
    srand((unsigned)time(nullptr));

    for (auto i = 0; i < iterations_count; i++)
    {
        switch (rand() % 2)
        {
            case 0:
                try
                {
                    switch (rand() % 3)
                    {
                        case 0:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::first_fit);
                        case 1:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_best_fit);
                        case 2:
                            the_same_subject->set_fit_mode(allocator_with_fit_mode::fit_mode::the_worst_fit);
                    }

                    allocated_blocks.push_front(allocator->allocate(sizeof(void *) * (rand() % 251 + 50)));
                    std::cout << "allocation succeeded" << std::endl;
                }
                catch (std::bad_alloc const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 1:
                if (allocated_blocks.empty())
                {
                    std::cout << "No blocks to deallocate" << std::endl;

                    break;
                }

                auto it = allocated_blocks.begin();
                std::advance(it, rand() % allocated_blocks.size());
                allocator->deallocate(*it, 1);
                allocated_blocks.erase(it);
                std::cout << "deallocation succeeded" << std::endl;
                break;
        }
    }

    while (!allocated_blocks.empty())
    {
        auto it = allocated_blocks.begin();
        std::advance(it, rand() % allocated_blocks.size());
        allocator->deallocate(*it, 1);
        allocated_blocks.erase(it);
        std::cout << "deallocation succeeded" << std::endl;
    }
}


int main(
    int argc,
    char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}