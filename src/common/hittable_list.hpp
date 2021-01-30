#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include "hittable.hpp"
#include <memory>
#include <vector>

class HittableList: public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);
    virtual bool hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const override;
};

HittableList::HittableList(std::shared_ptr<Hittable> object)
{
    add(object);
}

void HittableList::clear()
{
    objects.clear();
}

void HittableList::add(std::shared_ptr<Hittable> object)
{
    objects.push_back(object);
}

bool HittableList::hit(const Ray& ray, double min_parameter, double max_parameter, HitRecord& record) const
{
    HitRecord temp_record;
    bool hit_anything = false;
    auto closest_so_far = max_parameter;

    for (const auto& object: objects)
    {
        if (object->hit(ray, min_parameter, closest_so_far, temp_record))
        {
            hit_anything = true;
            closest_so_far = temp_record.parameter;
            record = temp_record;
        }
    }

    return hit_anything;
}

#endif // HITTABLE_LIST_HPP