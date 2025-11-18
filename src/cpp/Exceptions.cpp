#include "../Exceptions.hpp"

std::string FileException::Format(const std::string &f) {
  std::ostringstream os;
  os << "\nFile error: cannot open file '" << f << "'";
  return os.str();
}

std::string TextureFileExceptionOutOfBounds::Format(int idx) {
  std::ostringstream os;
  os << "\nTexture loader error: index " << idx << " does not exist: bad index";
  return os.str();
}

std::string TextureFileExceptionCorrupted::Format(const std::string &f) {
  std::ostringstream os;
  os << "\nTexture loader error: file '" << f << "' is corrupted and cannot be opened";
  return os.str();
}

std::string TextureFileExceptionExtension::Format(const std::string &f) {
  std::ostringstream os;
  os << "\nTexture loader error: file '" << f << "' has an unrecognised extension (must be a .png)";
  return os.str();
}

std::string TextureFileExceptionEmptyArray::Format(int index) {
  std::ostringstream os;
  os << "\nTexture loader error: texture array '"<< index << "' is empty";
  return os.str();
}

std::string TextureFileExceptionNoSuchFile::Format(int index, const std::string& filename) {
  std::ostringstream os;
  os << "\nTexture loader error: no such texture('"<<filename<<"') exists on array of index '"<< index<<"'";
  return os.str();
}