#include "OrderBook.h"


OfferId OrderBookAbseil::add_bid(Price price, Qty qty) {
    offers_.insert({offer_id_, {price, qty}});
    return offer_id_++;
}

OfferId OrderBookAbseil::add_ask(Price price, Qty qty) {
    offers_.insert({offer_id_, {price, qty}});
    return offer_id_++;
}
