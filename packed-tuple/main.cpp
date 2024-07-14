#include <array>
#include <cstdint>
#include <iostream>
#include <cassert>

template <std::uint8_t... BitSizes> struct TotalBits;

template <std::uint8_t First, std::uint8_t... Rest>
struct TotalBits<First, Rest...> {
    static constexpr std::uint8_t value = First + TotalBits<Rest...>::value;
};

template <> struct TotalBits<> {
    static constexpr std::uint8_t value = 0;
};

template <std::uint8_t Bits> struct TypeDeducer;

template <> struct TypeDeducer<1> {
    using type = bool;
};

template <std::uint8_t Bits> struct TypeDeducer {
    using type = std::conditional_t<
        (Bits <= 8), std::uint8_t,
        std::conditional_t<
            (Bits <= 16), u_int16_t,
            std::conditional_t<(Bits <= 32), u_int32_t, u_int64_t>>>;
};

template <std::uint8_t Bits>
using TypeDeducer_t = typename TypeDeducer<Bits>::type;

template <std::uint8_t... BitSizes> class BitPackedTuple {
    constexpr static std::uint8_t total_bits = TotalBits<BitSizes...>::value;
    constexpr static std::uint8_t byte_size = (total_bits + 7) / 8;

    std::uint8_t data[byte_size];

  public:
    template <std::uint8_t Index>
    __attribute__((always_inline)) inline void set(bool value) {
        constexpr std::uint8_t offset = calculate_offset<Index, BitSizes...>();
        constexpr std::uint8_t bit_idx = offset % 8;

        data[offset / 8] =
            data[offset / 8] & ~(1 << bit_idx) | (value * (1 << bit_idx));
    }

    template <std::uint8_t Index, std::uint8_t Shift = 0>
    __attribute__((always_inline)) inline void set(auto value) {
        constexpr std::uint8_t offset = calculate_offset<Index, BitSizes...>();
        constexpr std::uint8_t bits = get_nth_bit_size<Index, BitSizes...>();

        if constexpr (Shift == 0) {
            constexpr std::uint8_t mask = get_mask(bits);
            constexpr std::uint8_t data_shift = offset % 8;
            constexpr std::uint8_t value_shift = 0;

            data[offset / 8] = data[offset / 8] & ~(mask << data_shift) |
                               ((mask & (value >> value_shift)) << data_shift);

            set<Index, 1>(value);
            return;
        }

        if constexpr (bits + offset % 8 > Shift * 8) {
            static_assert(Shift <= 8);
            constexpr std::uint8_t mask =
                get_mask(bits + offset % 8 - Shift * 8);
            constexpr std::uint8_t value_shift = Shift * 8 - offset % 8;

            data[offset / 8 + Shift] = data[offset / 8 + Shift] & ~mask |
                                       ((mask & (value >> value_shift)));

            set<Index, Shift + 1>(value);
        }
    }

    template <std::uint8_t Index> inline auto get() const {
        constexpr std::uint8_t offset = calculate_offset<Index, BitSizes...>();
        constexpr std::uint8_t bits = get_nth_bit_size<Index, BitSizes...>();
        using T = TypeDeducer_t<bits>;
        T value = 0;

        constexpr std::uint8_t mask = get_mask(bits);
        constexpr std::uint8_t data_shift = offset % 8;
        constexpr std::uint8_t value_shift = 0;

        value |= (data[offset / 8] & (mask << data_shift)) >> data_shift;

        if constexpr (bits + offset % 8 > 1 * 8) {
            value |=
                (data[offset / 8 + 1] & get_mask(bits - 1 * 8 + offset % 8))
                << (1 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 2 * 8) {
            value |=
                (data[offset / 8 + 2] & get_mask(bits - 2 * 8 + offset % 8))
                << (2 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 3 * 8) {
            value |=
                (data[offset / 8 + 3] & get_mask(bits - 3 * 8 + offset % 8))
                << (3 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 4 * 8) {
            value |=
                (data[offset / 8 + 4] & get_mask(bits - 4 * 8 + offset % 8))
                << (4 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 5 * 8) {
            value |=
                (data[offset / 8 + 5] & get_mask(bits - 5 * 8 + offset % 8))
                << (5 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 6 * 8) {
            value |=
                (data[offset / 8 + 6] & get_mask(bits - 6 * 8 + offset % 8))
                << (6 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 7 * 8) {
            value |=
                (data[offset / 8 + 7] & get_mask(bits - 7 * 8 + offset % 8))
                << (7 * 8 - offset % 8);
        }
        if constexpr (bits + offset % 8 > 8 * 8) {
            value |=
                (data[offset / 8 + 8] & get_mask(bits - 8 * 8 + offset % 8))
                << (8 * 8 - offset % 8);
        }
        return value;
    }

  private:
    static constexpr std::uint8_t get_mask(std::uint8_t bits) {
        return (1 << std::min(bits, static_cast<std::uint8_t>(8))) - 1;
    }

    template <std::uint8_t Index, std::uint8_t First, std::uint8_t... Rest>
    static constexpr std::uint8_t calculate_offset() {
        if constexpr (Index == 0) {
            return 0;
        } else {
            return First + calculate_offset<Index - 1, Rest...>();
        }
    }

    template <std::uint8_t Index, std::uint8_t First, std::uint8_t... Rest>
    static constexpr std::uint8_t get_nth_bit_size() {
        if constexpr (Index == 0) {
            return First;
        } else {
            return get_nth_bit_size<Index - 1, Rest...>();
        }
    }
};

void test_bit_packed_tuple() {
    // Test with a single boolean value
    {
        BitPackedTuple<1> tuple;
        tuple.set<0>(true);
        assert(tuple.get<0>() == true);
        tuple.set<0>(false);
        assert(tuple.get<0>() == false);
    }

    // Test with multiple boolean values
    {
        BitPackedTuple<1, 1, 1> tuple;
        tuple.set<0>(true);
        tuple.set<1>(false);
        tuple.set<2>(true);
        assert(tuple.get<0>() == true);
        assert(tuple.get<1>() == false);
        assert(tuple.get<2>() == true);
    }

    // Test with mixed bit sizes
    {
        BitPackedTuple<1, 8, 4, 3> tuple;
        tuple.set<0>(true);
        tuple.set<1>(std::uint8_t(0xAB));
        tuple.set<2>(std::uint8_t(0xB));
        tuple.set<3>(std::uint8_t(0x5));

        assert(tuple.template get<0>() == true);
        assert(tuple.get<1>() == std::uint8_t(0xAB));
        assert(tuple.get<2>() == std::uint8_t(0xB));
        assert(tuple.get<3>() == std::uint8_t(0x5));
    }

    // Test with larger bit sizes
    {
        BitPackedTuple<16, 32, 8> tuple;
        tuple.set<0>(std::uint16_t(0xABCD));
        tuple.set<1>(std::uint32_t(0x12345678));
        tuple.set<2>(std::uint8_t(0x9A));

        assert(tuple.get<0>() == std::uint16_t(0xABCD));
        assert(tuple.get<1>() == std::uint32_t(0x12345678));
        assert(tuple.get<2>() == std::uint8_t(0x9A));
    }

    // Test with bit sizes that span multiple bytes
    {
        BitPackedTuple<10, 12, 14> tuple;
        tuple.set<0>(std::uint16_t(0x3FF)); // Max value for 10 bits
        tuple.set<1>(std::uint16_t(0xFFF)); // Max value for 12 bits
        tuple.set<2>(std::uint16_t(0x3FFF)); // Max value for 14 bits

        assert(tuple.get<0>() == std::uint16_t(0x3FF));
        assert(tuple.get<1>() == std::uint16_t(0xFFF));
        assert(tuple.get<2>() == std::uint16_t(0x3FFF));
    }

    // Test boundary conditions
    {
        BitPackedTuple<1, 2, 3, 4, 5, 6, 7, 8> tuple;
        tuple.set<0>(true);
        tuple.set<1>(std::uint8_t(0x3)); // 2 bits
        tuple.set<2>(std::uint8_t(0x7)); // 3 bits
        tuple.set<3>(std::uint8_t(0xF)); // 4 bits
        tuple.set<4>(std::uint8_t(0x1F)); // 5 bits
        tuple.set<5>(std::uint8_t(0x3F)); // 6 bits
        tuple.set<6>(std::uint8_t(0x7F)); // 7 bits
        tuple.set<7>(std::uint8_t(0xFF)); // 8 bits

        assert(tuple.get<0>() == true);
        assert(tuple.get<1>() == std::uint8_t(0x3));
        assert(tuple.get<2>() == std::uint8_t(0x7));
        assert(tuple.get<3>() == std::uint8_t(0xF));
        assert(tuple.get<4>() == std::uint8_t(0x1F));
        assert(tuple.get<5>() == std::uint8_t(0x3F));
        assert(tuple.get<6>() == std::uint8_t(0x7F));
        assert(tuple.get<7>() == std::uint8_t(0xFF));
    }

    std::cout << "All tests passed!" << std::endl;
}

int main() {
    test_bit_packed_tuple();
    return 0;
}
