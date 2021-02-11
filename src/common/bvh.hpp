#ifndef BVH_HPP
#define BVH_HPP

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

inline bool box_compare(const std::shared_ptr<Hittable> first, const std::shared_ptr<Hittable> second, int axis)
{
    AABB box_first;
    AABB box_second;

    if (!first->bounding_box(0, 0, box_first) || !second->bounding_box(0, 0, box_second))
    {
        std::cerr << "No bounding box in BVHNode constructor\n";
    }

    return box_first.min().coord[axis] < box_second.min().coord[axis];
}

bool box_x_compare(const std::shared_ptr<Hittable> first, const std::shared_ptr<Hittable> second)
{
    return box_compare(first, second, 0);
}

bool box_y_compare(const std::shared_ptr<Hittable> first, const std::shared_ptr<Hittable> second)
{
    return box_compare(first, second, 1);
}

bool box_z_compare(const std::shared_ptr<Hittable> first, const std::shared_ptr<Hittable> second)
{
    return box_compare(first, second, 2);
}

class BVHNode: public Hittable
{
public:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB box;
    
    BVHNode() {}
    BVHNode(const HittableList& list, double start_time, double end_time): BVHNode{list.objects, 0, list.objects.size(), start_time, end_time} {}
    BVHNode(const std::vector<std::shared_ptr<Hittable>>& src_objects, std::size_t start, std::size_t end, double start_time, double end_time);

    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
    virtual bool bounding_box(double start_time, double end_time, AABB& output_box) const override;
};

BVHNode::BVHNode(const std::vector<std::shared_ptr<Hittable>>& src_objects, std::size_t start, std::size_t end, double start_time, double end_time)
{
    auto objects = src_objects;

    int choosen_axis = random_int(0, 2);
    auto comparator = (choosen_axis == 0) ? box_x_compare : (choosen_axis == 1) ? box_y_compare : box_z_compare;

    std::size_t size = end - start;

    if (size == 1)
    {
        left = objects[start];
        right = objects[start];
    }
    else if (size == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else 
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else 
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        
        auto mid = start + size / 2;
        left = std::make_shared<BVHNode>(objects, start, mid, start_time, end_time);
        right = std::make_shared<BVHNode>(objects, mid, end, start_time, end_time);
    }

    AABB box_left;
    AABB box_right;

    if (!left->bounding_box(start_time, end_time, box_left) || !right->bounding_box(start_time, end_time, box_right))
    {
        std::cerr << "No bounding box in BVHNode constructor\n";
    }

    box = surrounding_box(box_left, box_right);
}

bool BVHNode::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    if (!box.hit(ray, min_parameter, max_parameter))
    {
        return false;
    }

    bool hit_left = left->hit(ray, min_parameter, max_parameter, record);
    bool hit_right = right->hit(ray, min_parameter, hit_left ? record.parameter : max_parameter, record);

    return hit_left || hit_right;
}

bool BVHNode::bounding_box(double start_time, double end_time, AABB& output_box) const 
{
    output_box = box;
    return true;
}

#endif // BVH_HPP