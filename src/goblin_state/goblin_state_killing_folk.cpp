#include "goblin_state.hpp"
#include "goblin_impl.hpp"


template<>
auto goblin_state_::KillingFolk::on_entry(GoblinBorn const &event, GoblinState &fsm) -> void {
    std::cout << "entering KillingFolk" << std::endl;
    this->kill_timer_.emplace(event.impl.get_executor());
    auto &timer = kill_timer_.get();
    timer.expires_from_now(boost::posix_time::seconds(1));

    // take a shared pointer to the impl, not the handle
    auto impl_ptr = event.impl.shared_from_this();
    timer.async_wait([impl_ptr](asio::error_code const &ec) {
        std::cout << "KillingFolk timer: " << ec.message() << std::endl;
        if (not ec) {
            impl_ptr->process_event(GoblinKilledSomeone{*impl_ptr});
        }
    });

}
