#if !defined SHAPES_HPP
#define SHAPES_HPP

//////////
// Point
//////////

struct Point
{
  Point() : x_(0), y_(0) {}
  Point(int x, int y) : x_(x), y_(y) {}
  int x_;
  int y_;
};

SD_MAKE_CLASSNAME(Point)

template <class SerDes>
bool WriteBody(SerDes& serdes, const std::string& name, const Point& object)
{
    WriteObject(serdes, "x", object.x_);
    WriteObject(serdes, "y", object.y_);
    return true;
}


//////////
// Shape
//////////
  
struct Shape
{
  virtual bool Write(SerDesOutBase& serdes, const std::string& name) const = 0;
  
  SD_DECLARE_ABSTRACT_CLASS 

};  

SD_MAKE_BASECLASS(Shape)




/////////////
// Triangle
/////////////

struct Triangle : public Shape
{
  Triangle()
  {
    point1_ = Point(0,0);
    point2_ = Point(1,1);
    point3_ = Point(1,0);
  }
    virtual bool Write(SerDesOutBase& serdes, const std::string& name) const
  {
    WriteObject(serdes, "Point1", point1_);
    WriteObject(serdes, "Point2", point2_);
    WriteObject(serdes, "Point3", point3_);
    return true;
  }  
  
  Point point1_;
  Point point2_;
  Point point3_;

  SD_DECLARE_CLASS(Triangle,1010)
};  


////////////
// Circle
////////////

struct Circle : public Shape
{
  Circle()
  {
    center_ = Point(0,0);
    radius_ = 1;
  }
     
  virtual bool Write(SerDesOutBase& serdes, const std::string& name) const
  {
    WriteObject(serdes, "Center", center_);
    WriteObject(serdes, "Radius", radius_);
    return true;
  }  
  
  Point center_;
  int radius_;

  SD_DECLARE_CLASS(Circle,1020)
};  



//////////////
// Rectangle
//////////////

struct Rectangle : public Shape
{
  Rectangle()
  {
    point1_ = Point(0,0);
    point2_ = Point(1,1);
  }
     
  virtual bool Write(SerDesOutBase& serdes, const std::string& name) const
  {
    WriteObject(serdes, "Point1", point1_);
    WriteObject(serdes, "Point2", point2_);
    return true;
  }  
  
  Point point1_;
  Point point2_;

  SD_DECLARE_CLASS(Rectangle,1030)
};  


#endif /* SHAPES_HPP */ 

