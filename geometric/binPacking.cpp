#include <algorithm>
#include <list>
#include "geometric/binPacking.h"

using glm::vec2;

namespace BinPacking
{
std::vector<AABBoxPtr> PackFCNR(std::vector<AABBoxPtr>& bboxes, float binWidth, float binHeight)
{
    std::vector<AABBoxPtr> packedBoxes;

    std::sort(bboxes.begin(), bboxes.end(),
         [](const AABBoxPtr& i, const AABBoxPtr& j) { return i->height < j->height; } );

    struct FCNR_level
    {
        std::list<SAABBox2D*> floor;
        std::list<SAABBox2D*> ceiling;
        float ceilHeight;
        float floorHeight;
    };

    std::list<FCNR_level> levels;
    for(int i=bboxes.size()-1; i>=0; i--)
    {
        SAABBox2D& bbx = *bboxes[i];
        if(bbx.width > binWidth)
            continue;

        bool added = false;
        for(auto &lvl : levels)
        {
            SAABBox2D* rmost_fl = lvl.floor.back();

            float x = rmost_fl->GetRight();

            if(binWidth - x >= bbx.width)
            {
                bbx.position = vec2(x + bbx.width * 0.5f, lvl.floorHeight + bbx.height * 0.5f);
                bool intersects = false;
                for(auto& ceilBBX : lvl.ceiling)
                {
                    if(ceilBBX->Intersects(bbx))
                    {
                        intersects = true;
                        break;
                    }
                }
                if(!intersects)
                {
                    lvl.floor.push_back(&bbx);
                    added = true;
                    break;
                }
            }

            if(!lvl.ceiling.empty())
            {
                SAABBox2D* lmost_cl = lvl.ceiling.back();
                x = lmost_cl->GetLeft();
            } else {
                x = binWidth;
            }

            if(x >= bbx.width)
            {
                bbx.position = vec2(x - bbx.width * 0.5f, lvl.floorHeight + lvl.ceilHeight - bbx.height * 0.5f);
                bool intersects = false;
                for(auto& floorBBX : lvl.floor)
                {
                    if(floorBBX->Intersects(bbx))
                    {
                        intersects = true;
                        break;
                    }
                }
                if(!intersects)
                {
                    lvl.ceiling.push_back(&bbx);
                    added = true;
                    break;
                }
            }
        }

        if(!added)
        {
            FCNR_level* prev = nullptr;
            if(!levels.empty())
            {
                prev = &levels.back();
                if(prev->ceilHeight + prev->floorHeight + bbx.height > binHeight)
                    continue;
            }

            levels.push_back(FCNR_level());
            FCNR_level &lvl = levels.back();

            lvl.ceilHeight = bbx.height;
            lvl.floorHeight = (prev ? prev->ceilHeight + prev->floorHeight : 0.0f);
            lvl.floor.push_front(&bbx);
            bbx.position = vec2(bbx.width * 0.5f, lvl.floorHeight + bbx.height * 0.5f);
        }
        packedBoxes.emplace_back(std::move(bboxes[i]));
        bboxes.erase(bboxes.begin() + i);
    }

    return packedBoxes;
}
} //namespace BinPacking