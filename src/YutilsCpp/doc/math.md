# class Math

This class is defined in [math.hpp](../math.hpp).

## Public member functions

- [class Math](#class-math)
  - [Public member functions](#public-member-functions)
    - [create](#create)
    - [arc_curve](#arccurve)
    - [bezier](#bezier)
    - [degree](#degree)
    - [distance](#distance)
    - [line_intersect](#lineintersect)

### create

```c++
static std::shared_ptr<Math> create()
```
Create class Math object. If failed, it will return nullptr.

### arc_curve

```c++
std::vector<std::pair<double, double>>
arc_curve(double x, double y, double cx, double cy, double angle)
```
Converts arc data to bezier curves.  
This function may **throw std::invalid_argument**  
- x & y: the arc starting point
- cx & cy: the arc center<br />
           (= orientation point to keep the same distance to all arc points)
- angle: the angle in degree of the arc. **CANNOT be zero.**

For each 90 degrees, one curve is generated, so a maximum of 4 curves can span a circle.  
Curves are 3rd order bezier curves.  
It returns a vector conatins pairs.  
Each pair is one of the control points of a bezier curve.  
Every four pairs describe a bezier curve.  

### bezier

```c++
std::tuple<double, double, double>
bezier(double pct,
       std::vector<std::tuple<double, double, double>> &pts,
       bool is3D = false);
```
Calculates a point on a bezier curve of any order.  
This function may **throw std::invalid_argument**  
- pct: The position on the curve **in range 0<=x<=1**.
- pts: A vector contains tuples,<br />
       each tuple containing 3 numbers as curve point.<br />
       the size of this vector **MUST greater than 1**.<br />
- is3D: It indicates pts is 3D or not.<br />
        If is3D is set to false,<br />
        the third number of tuples of pts will be ignored,<br />
        and the third number of returned tuple always is zero.

### degree

```c++
double degree(double x1, double y1, double z1,
              double x2, double y2, double z2);
```
Calculates the degree between vectors x1|y1|z1 and x2|y2|z3.

### distance

```c++
double distance(double x, double y, double z = 0.);
```
Calculates length of given vector.

### line_intersect

```c++
std::pair<double, double>
line_intersect(double x0, double y0,
               double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               bool strict);
```
Calculates intersection point of two lines.  
This function may **throw std::invalid_argument**  
Lines **MUSTN'T have zero length**.
- x0, y0, x1, y1: points of line 1
- x2, y2, x3, y3: points of line 2
- strict: determining the intersection has to be located on the lines.<br />
          x, y can be the intersection point.<br />
          If both lines are parallel, it will return empty pair.<br />
          If strict is true and there's no intersection on the strict length lines,<br />
          it will return inf (std\:\:numeric_limits::infinity())

