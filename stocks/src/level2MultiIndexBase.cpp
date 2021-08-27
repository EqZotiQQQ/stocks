#include "level2MultiIndexBase.h"

namespace multiinexBase {

    OfferID Level2MultiIndexBase::add_order(Count quantity, Price price, OFFER offer_type) noexcept {
        if (offer_type == OFFER::BID) {    // выставляем на продажу

        } else { // выставляем на покупку

        }
        return offer_id_++;
    }


    bool Level2MultiIndexBase::close_order(Count quantity, OfferID id) noexcept {
        return false;
    }

}
