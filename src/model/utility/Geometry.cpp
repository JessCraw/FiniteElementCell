#include "Geometry.h"

#include <cmath>

Coordinates Geometry::calcPointOnLineThroughExternalPointGivingPerpendicularLine(Coordinates C, Coordinates S,
                                                                                 Coordinates En)
{
    // std::vector<float> ans;
    Coordinates D = {-1, -1, -1};
    // line SE, point D is on line SE and gives a perpendicular line through external point C

    // dot product of CD.SE=0 is perpendicular
    // Dy=mDx+C for m and c of line SE if point is on the line
    // so by substitution find Dx and Dy.

    std::vector<float> ans = equationOfLineThroughTwoPoints(S.x, S.y, En.x, En.y);

    if (ans.size() == 3) {
        // then its a straight line in x axis so do x = my+c

        if (ans[0] == 0) {
            D.x = ans[1];

            if (En.y - S.y != 0)
                D.y = (((En.y - S.y) * C.y) - (D.x * (En.x - S.x)) + (C.x * (En.x - S.x))) /
                      (En.y - S.y);  //-(((D.x-C.y)*(En.y-S.y))/(En.x-S.x))+C.x;
            else
                D.y = 0;

        } else {
            std::cout << "oh it appears you can get a reverse equation of line when m isnt 0" << std::endl;
        }
    } else {
        // then treat as normal
        // Dy = (m*Dx)+c;

        // can jump ahead if m=0
        if (ans[0] == 0) {
            D.y = ans[1];

            D.x = -(((D.y - C.y) * (En.y - S.y)) / (En.x - S.x)) + C.x;
        } else {
            // Dy = mDx+c
            // substitute into vec equ..
            //(Dx-Cx)*SEx+((mDx+c)-Cy)*SEy=0
            // Dx =( -(((mDx+c)-Cy)*SEy)/SEx)+Cx

            D.x = (((En.x - S.x) * C.x) + ((En.y - S.y) * (C.y - ans[1]))) / ((En.x - S.x) + ((En.y - S.y) * ans[0]));
            D.y = (ans[0] * D.x) + ans[1];
        }
        // So,
    }

    D.z = 0.0f;
    return (D);
}

std::vector<float> Geometry::equationOfLineThroughTwoPoints(float x1, float y1, float x2, float y2)
{
    float m, c;
    std::vector<float> ans;
    int flag = 0;

    if ((x2 - x1) != 0) {
        m = (y2 - y1) / (x2 - x1);
        c = y1 - m * x1;
    } else {
        // need to swap around = so its x = my+c, as the x's are the same so equation is flipped to get line x=c
        m = 0;
        c = x1;
        flag = 1;
    }

    // if size of ans = 3 then reverse equation so have x = my+c
    ans.push_back(m);
    ans.push_back(c);
    if (flag == 1)
        ans.push_back(5.0f);

    return (ans);
}

float Geometry::reverseAngle(float angle) { return std::fmod((angle + M_PI), (2 * M_PI)); }

bool areEqual(float a, float b)
{
    return std::abs(a - b) < 1e-6F;  // Adjust epsilon as needed for precision requirements
}

bool Geometry::doLinesIntersect(const Coordinates &line1Start, const Coordinates &line1End,
                                const Coordinates &line2Start, const Coordinates &line2End)
{
    // Calculate direction vectors of the lines
    // Point dir1 = {line1.end.x - line1.start.x, line1.end.y - line1.start.y, line1.end.z - line1.start.z};
    Coordinates dir1 = {line1End.x - line1Start.x, line1End.y - line1Start.y, line1End.z - line1Start.z};
    Coordinates dir2 = {line2End.x - line2Start.x, line2End.y - line2Start.y, line2End.z - line2Start.z};

    // Calculate cross product of direction vectors
    Coordinates cross = {dir1.y * dir2.z - dir1.z * dir2.y, dir1.z * dir2.x - dir1.x * dir2.z,
                         dir1.x * dir2.y - dir1.y * dir2.x};

    // If cross product is close to zero, lines are parallel
    if (areEqual(cross.x, 0.0) && areEqual(cross.y, 0.0) && areEqual(cross.z, 0.0)) {
        // Check if the start point of one line lies on the other line
        if (areEqual(line1Start.x, line2Start.x) && areEqual(line1Start.y, line2Start.y) &&
            areEqual(line1Start.z, line2Start.z)) {
            return true;
        }
        // Check if the start point of one line lies on the other line
        if (areEqual(line1Start.x, line2End.x) && areEqual(line1Start.y, line2End.y) &&
            areEqual(line1Start.z, line2End.z)) {
            return true;
        }
        // Check if the end point of one line lies on the other line
        if (areEqual(line1End.x, line2Start.x) && areEqual(line1End.y, line2Start.y) &&
            areEqual(line1End.z, line2Start.z)) {
            return true;
        }
        // Check if the end point of one line lies on the other line
        if (areEqual(line1End.x, line2End.x) && areEqual(line1End.y, line2End.y) && areEqual(line1End.z, line2End.z)) {
            return true;
        }
        return false;  // Lines are parallel but not coincident
    }

    // Lines are not parallel, check if they intersect
    return true;
}
