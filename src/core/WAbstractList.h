#pragma once

#include "utils/WCommonScript.h"
#include "VersionFileController.h"
#include <algorithm>
#include "Scheduler/WTask.h"
#include "FileContainer/MemReadable.h"
#include "FileContainer/MemWritable.h"
#include "core/WPair.h"
#include "WElement.h"
#include "core/pointer/SharedPtrThreadSafe.h"
#include "core/pointer/Pointer.h"
#include "core/Error.h"

namespace WAbstractList {
    template <class T>
    using Ptr = SharedPtrThreadSafe<T>;

    template <class Iter>
    auto isSorted(Iter begin, Iter end) noexcept -> bool
    {
        auto before = begin;
        begin ++;
        for (; begin != end; ) {
            if (*before > *begin)
                return false;
            before ++;
            begin ++;
        }

        return true;
    };

    template<class T>
    auto isSorted(const T& list) -> bool
    {
        auto b1 = list.begin();
        auto b2 = b1 + 1;
        const auto end = list.end();

        for (; b2 != end; b1++, b2++) {
            if (b1 > b2)
                return false;
        }

        return true;
    };

    template <class T>
    auto sort (T begin, T end) -> void
    {
        std::sort(begin, end);
    }

    template <class T, class F>
    auto sort (T begin, T end, Fn<bool(const F& v1, const F& v2)> cmp) -> void
    {
        std::sort(begin, end, cmp);
    }

    /**
     * \tparam Iterator The iterator
     * \param begin The begin iterator
     * \param end The end iterator
     * \return end iff object is not between [begin, end)
     * \param cmp needs to return
     *              0 in case v1 == v2
     *              1 in case v1 > v2
     *              -1 in case v1 < v2
     * */
     /*
    template <class Iterator, class T, bool descending = false>
    auto binary_search (
            Iterator begin,
            Iterator end,
            const T& object,
            Fn<int(const T& v1, const T& v2)> cmp = [](const T& v1, const T& v2) {
                if (v1 == v2) return 0;
                if (v1 > v2)  return 1;
                if (v1 < v2)  return -1;
            }
        ) -> Iterator
    {
        long long left = 0;
        long long right = end - begin;

        while (left <= right) {
            auto mid = left + (right - left) / 2;

            const int result = cmp (*(begin + mid), object);

            if (result == 0)
                return begin + mid;

            if (result > 0) {
                if constexpr (descending)
                    right = mid - 1;
                else
                    left = mid + 1;
            } else {
                if constexpr (descending)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }

        return end;
    }*/

    /**
     * \tparam Iterator The iterator
     * \param begin The begin iterator
     * \param end The end iterator
     * \return end iff object is not between [begin, end)
     * \param cmp needs to return true in case v1 >= v2
     * \tparam lowToHigh true if the list if ascending [ordine crescente]
     * */
    template <class Iterator, class T, bool lowToHigh = true>
    auto binary_search(
            Iterator begin,
            Iterator end,
            const T& object,
            Fn<bool(const T& v1, const T& v2)> cmp = [](const T& v1, const T& v2) {
                return v1 >= v2;
            }
        ) -> Iterator
    {
        long long left = 0;
        long long right = end - begin - 1;

        Iterator result = end;

        while (left <= right) {
            auto mid = left + (right - left) / 2;

            if (*(begin + mid) == object) {
                result = begin + mid;
                break;
            }

            if (cmp(*(begin + mid), object)) {
                if constexpr (lowToHigh)
                    right = mid - 1;
                else
                    left = mid + 1;
            } else {
                if constexpr (lowToHigh)
                    left = mid + 1;
                else
                    right = mid - 1;
            }
        }

        if (result == end)
            return end;

        for (;;) {
            WDebug(false, "Cicle")
            if (result == begin)
                return result;

            result --;
            if (*result != object) {
                WDebug(false, "return");
                return result + 1;
            }
        }
    }

    template <class List>
    class Reverse
    {
        List &_list;
    public:
        explicit Reverse(List& list) : _list(list) {};

        auto rbegin() const { return _list.begin(); };
        auto rend() const { return _list.end(); };

        auto rbegin() { return _list.begin(); };
        auto rend() { return _list.end(); };

        auto begin() { return _list.rbegin(); }
        auto end() { return _list.rend(); }

        auto begin() const { return _list.rbegin(); }
        auto end() const { return _list.rend(); }
    };

    /**
     * \tparam Iterator iterator for the list
     * \tparam T the object contained in the list
     * \tparam lowToHigh true if the list should be order from low item to high item
     *
     * \param begin the begin of the list
     * \param end the end of the list
     * \param cmp should return true iff v1 >= v2
     * */
    template <class Iterator, class T, bool lowToHigh>
    auto createHeap(Iterator begin, Iterator end, Fn<bool(const T& v1, const T& v2)> cmp) -> bool
    {
        if (lowToHigh)
            WAbstractList::sort(begin, end, cmp);
        W_ASSERT(0);
    };

    template <template <class T> class List, class T2>
    static auto write (
            WritableAbstract &writable,
            const List<T2> &list,
            Fn<Error(
                    WritableAbstract &writable,
                    const T2&
            )> save
    ) -> Error
    {
        unsigned size = list.size();

        WDebug(false, "Saving: " << typeid(T2).name() << size);

        if (auto err = writable.write(size)) {
            return err;
        }

        for (const auto &ref: std::as_const(list)) {
            if (auto err = save(writable, ref))
                return err;
        }

        return Error::makeOk();
    }

    template <template <class T> class List, class T2>
    auto write (WritableAbstract& writable, const List<T2>& list) -> Error
    {
        return WAbstractList::write<List, T2>(writable,
                                              list,
                                              [] (WritableAbstract &writable, const T2 &object) -> Error {
            return T2::write (writable, object);
        });
    }

    template <template <class T> class List, class T2>
    static auto load  (
            const VersionFileController &versionController,
            ReadableAbstract &readable,
            Fn<WPair<Error, T2>(
                    const VersionFileController &versionController,
                    ReadableAbstract &readable
            )> func
    ) -> WPair<Error, List<T2>>
    {
        List<T2> result;

        switch (versionController.getVersionWListFast()) {
            case 0:
                int element;

                if (auto err = readable.read(element)) {
                    return {err, {}};
                }

                result.reserve(element);

                for (int i = 0; i < element; i++) {
                    auto [res, data] = func (versionController, readable);
                    if (res)
                        return {res, {}};

                    result.append(
                            std::move (data)
                    );
                }
                return {Error::makeOk(), result};
            default:
                return {Error::makeErrVersion(), {}};
        }
        return {Error::makeErrVersion(), {}};
    }

    template <template <class T> class List, class T2>
    auto load (const VersionFileController& version, ReadableAbstract& readable) -> WPair<Error, List<T2>>
    {
        return WAbstractList::load<List, T2>(
                version,
                readable,
                [](const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, T2> {
                    return T2::load(versionController, readable);
                }
        );
    }

    template <template <typename Ty> class List, class T>
    auto loadMultiThread(
            const VersionFileController &versionController,
            ReadableAbstract &readable,
            const Fn<Ptr<WTask> (
                    Fn<void()>
            )> &startNewThread,
            const Fn<void(List<T>& list, int numberOfElement)> &reserveUnsafe,
            const Fn<void(List<T>& list, T&& object, int index)> &insertUnsafe
    ) noexcept -> WPair<Error, List<T>>
    {
        if (versionController.getVersionWAbstractList() != 0)
            return {Error::makeErrVersion(), {}};

        int i;
        List<T> result;
        List<Ptr<WTask>> threads;
        List<UnsignedLong> seek;
        auto needToAbort = Error::makeOk();

        {
            // load seek array
            auto [tmp, seekLoad] = WAbstractList::load<List, UnsignedLong>(versionController, readable);

            if (tmp)
                return {tmp, {}};

            seek = std::move(seekLoad);
        }

        if (!seek.size()) {
            return {Error::makeOk(), {}};
        }

        MemReadable readers[seek.size()];

        char rawData[seek.last().value()];
        if (auto err = readable.read (rawData, seek.last()))
            return {err, {}};

        for (i = 0; i < seek.size(); i++) {
            readers[i]
                    .setData(
                            rawData + ((i == 0) ? 0 : seek[i - 1]),
                            seek[i]
                    );
        }

        reserveUnsafe(result, seek.size());

        for (i = 0; i < seek.size(); i++) {
            threads.append(
                    startNewThread (
                            [&versionController, &needToAbort, &readers, i, &result, &insertUnsafe]() {
                                auto [res, data] = T::load (versionController, readers[i]);

                                if (res) {
                                    needToAbort = res;
                                    return;
                                }

                                insertUnsafe(result, std::forward<T>(data), i);
                            }
                    )
            );
        }

        for (auto &thread: threads) {
            thread->join();

            thread.release();
        }

        if (needToAbort)
            return {needToAbort, {}};

        return {Error::makeOk(), result};
    };

    template<template <class T> class List, class T2>
    auto writeMultiThread(
            WritableAbstract &writable,
            const List<T2> &list,
            const Fn<Ptr<WTask>(
                    Fn<void()>
            )> &startNewThread
    ) noexcept -> Error
    {
        int i = 0;

        std::vector<Ptr < WTask>> threads;
        MemWritable w[list.size()];
        Error needToAbort = Error::makeOk();

        threads.reserve(list.size());

        // create threads with corresponding data
        for (const auto &ref: std::as_const(list)) {
            threads.push_back(startNewThread ([ref, &needToAbort, &w, i] {
                                               if (auto err = T2::write (w[i], ref))
                                                   needToAbort = err;
                                           }
            ));
            i++;
        }

        // join the threads
        for (auto &thread: threads) {
            thread->join();
            thread.release();
        }

        for (auto& thread: threads) {
            thread.release();
        }

        if (needToAbort)
            return needToAbort;

        // write to writer the seek information
        {
            List<UnsignedLong> seek(list.size());
            size_t seekDelta = 0;

            for (i = 0; i < list.size(); i++) {
                seekDelta += w[i].getCurrentSize();
                seek.append(UnsignedLong(seekDelta));
            }

            if (auto err = WAbstractList::write<List, UnsignedLong>(writable, seek))
                return err;
        }

        // we merge all the single writer in the main writer
        for (MemWritable& singleWriter: w) {
            if (auto err = singleWriter.merge(writable))
                return err;
        }

        for (i = 0; i < list.size(); i++) {
            // TODO: optimize this copy
            if (auto err = w[i].merge(writable))
                return err;
        }

        return Error::makeOk();
    };
};
