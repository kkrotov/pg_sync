#include "BDbResult.h"
#include <cstring>

BDbResult::BDbResult(PGresult * res) {
    this->res = res;
    this->index = -1;
    this->count = PQntuples(res);
}

BDbResult::~BDbResult() {
    if (res != 0)
        PQclear(res);
}

size_t BDbResult::size() {
    return (size_t)count;
}

size_t BDbResult::position() {
    return (size_t)index;
}

char * BDbResult::get(int r, int f) {
    return PQgetvalue(res, r, f);
}

bool BDbResult::is_null(int f) {
    return (bool)PQgetisnull(res, index, f);
}

char * BDbResult::get(int f) {
    return PQgetvalue(res, index, f);
}

int BDbResult::get_i(int f) {
    return atoi(PQgetvalue(res, index, f));
}

double BDbResult::get_d(int f) {
    return atof(PQgetvalue(res, index, f));
}

long long int BDbResult::get_ll(int f) {
    return atoll(PQgetvalue(res, index, f));
}

bool BDbResult::get_b(int f) {
    return 't' == *PQgetvalue(res, index, f);
}

string BDbResult::get_s(int f) {
    return PQgetvalue(res, index, f);
}

void BDbResult::fill_cs(int f, char * str, size_t size) {
    strncpy(str, PQgetvalue(res, index, f), size - 1);
}

bool BDbResult::next() {
    index++;
    return count > 0 && index < count;
}

bool BDbResult::previous() {
    index--;
    return count > 0 && index < count;
}

bool BDbResult::first() {
    if (count > 0) {
        index = 0;
        return true;
    } else
        return false;
}

bool BDbResult::last() {
    if (count > 0) {
        index = count - 1;
        return true;
    } else
        return false;
}

void BDbResult::force_free() {
    if (res != 0) {
        PQclear(res);
        res = 0;
    }
}
