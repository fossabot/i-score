#pragma once
#include <type_traits>
#include <iscore/Settings.hpp>
#include <Space/area.hpp>

namespace spacelib
{

template<int size = 800, int side = 5>
class square_approx
{
    public:
        square_approx(const space<2>& s, int dim) // TODO  require concept bounded_space with min/max
        {

        }

        class iterator
        {
            public:
                constexpr iterator() = default;
                constexpr iterator(int p): pos{p} { }

                constexpr iterator operator++()
                { return ++pos, *this; }
                constexpr iterator operator++(int)
                { return ++pos, *this; }

                constexpr int operator*() const
                { return pos * side; }

                constexpr bool operator!=(iterator other) const
                { return other.pos != pos; }

            private:
                int pos = 0;
        };


        constexpr iterator begin()
        { return iterator(); }
        constexpr iterator end()
        { return iterator(size/side); }
};


// Iterates on all the dimensions of the space and call
// a function on each point
template<typename Space, typename Approx, typename Fun>
class dimension_iterator
{
        const Space& m_space;
        Fun m_fun;
        Approx m_approx;
    public:
        dimension_iterator(
                const Space& s,
                Approx approx,
                Fun&& f):
            m_space{s},
            m_fun{std::move(f)},
            m_approx{approx}
        {
        }


        void rec(GiNaC::exmap& var_map)
        {
            rec_impl<0>(var_map);
        }

    private:
             template<int Dimension,
                    std::enable_if_t< Dimension < std::decay_t<Space>::dimension - 1 >* = nullptr >
        void rec_impl(GiNaC::exmap& var_map)
        {
            for(int i : m_approx)
            {
                var_map[m_space.variables()[Dimension]] = i;
                rec_impl<Dimension+1>(var_map);
            }
        }

             template<int Dimension,
                    std::enable_if_t< Dimension == std::decay_t<Space>::dimension - 1 >* = nullptr >
        void rec_impl(GiNaC::exmap& var_map)
        {
            for(int i : m_approx)
            {
                var_map[m_space.variables()[Dimension]] = i;
                m_fun(var_map);
            }
        }
};


template<typename Size, typename Device>
class square_renderer
{
    public:
        Device render_device;
        double side = 5.; // square size
        Size size; //complete size.

        void render(const area& a, const space<2>& s)
        {
            GiNaC::exmap var_map;
            auto eqn = a.map_to_space(s);

            auto dim = make<dimension_iterator>(s, square_approx<800, 5>(s, 0),
            [&] (const GiNaC::exmap& map) {
                if(area::check(eqn, a.parameters(), map))
                {
                    render_device.add(
                                GiNaC::ex_to<GiNaC::numeric>(map.at(s.variables()[0])).to_double() - side/2.,
                            GiNaC::ex_to<GiNaC::numeric>(map.at(s.variables()[1])).to_double() - side/2.,
                            side, side);
                }
            });

            dim.rec(var_map);
        }
};
}
