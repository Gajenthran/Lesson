#include <vector>


class GSImage {
public:
  GSImage(char const* filename, int w, int h);
  GSImage(char const* filename);
  GSImage();

  void save_png(char const* filename);
  auto pixels() { return pixels_.data(); }
  auto w() const { return w_; }
  auto h() const { return h_; }

private:
  std::vector<uint8_t> pixels_;
  int w_;
  int h_;
};

typedef struct GSView GSView;
struct GSView {
  int x;
  int y;
  int w;
  int h;
  GSImage* image;
};