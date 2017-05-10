#ifndef DISPARITY_HPP
#define DISPARITY_HPP

enum class disp_t {BM, BMWLS};

class disparity {
private:

public:
  disparity();
  disparity(disp_t type);
};

#endif
