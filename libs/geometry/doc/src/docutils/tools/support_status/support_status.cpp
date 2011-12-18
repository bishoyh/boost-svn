#include <iostream>

#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>

#define BOOST_GEOMETRY_IMPLEMENTATION_STATUS_BUILD true
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/convert.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/strategies/cartesian/distance_pythagoras.hpp>


typedef boost::geometry::cs::cartesian cartesian;

typedef boost::geometry::model::point<double, 2, cartesian> point_type;
typedef boost::geometry::model::linestring<point_type>      line_type;
typedef boost::geometry::model::polygon<point_type>         polygon_type;
typedef boost::geometry::model::box<point_type>             box_type;
typedef boost::geometry::model::ring<point_type>            ring_type;
typedef boost::geometry::model::segment<point_type>         segment_type;

typedef boost::mpl::vector<
    point_type,
    line_type,
    polygon_type,
    box_type,
    ring_type,
    segment_type
> types;


#define DECLARE_CHECK(algorithm) \
    template <typename G1, typename G2> \
    struct check_##algorithm: boost::geometry::dispatch::algorithm<G1, G2> \
    {};

DECLARE_CHECK(distance)
DECLARE_CHECK(convert)
DECLARE_CHECK(append)


template <template <typename, typename> class Dispatcher, typename G1 = void>
struct tester
{
    template <typename G2>
    void operator()(G2)
    {
        if (boost::is_base_of<boost::geometry::not_implemented_base, Dispatcher<G1, G2> >::type::value)
        {
            std::cout << "-\t";
        }
        else
        {
            std::cout << "OK\t";
        }
    }
};

template <template <typename, typename> class Dispatcher>
struct tester<Dispatcher, void>
{
    template <typename G>
    void operator()(G)
    {
        boost::mpl::for_each<types>(tester<Dispatcher, G>());
        std::cout << std::endl;
    }
};


int main()
{
    std::cout << "DISTANCE" << std::endl;
    boost::mpl::for_each<types>(tester<check_distance>());
    std::cout << std::endl;

    std::cout << "CONVERT" << std::endl;
    boost::mpl::for_each<types>(tester<check_convert>());
    std::cout << std::endl;

    return 0;
}
