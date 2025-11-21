#pragma once

#include <sstream>
#include <exception>
#include <string>

// generic exception(msg)
class AppException : public std::exception {
public:
  explicit AppException(std::string msg) : msg_(std::move(msg)) {}
  const char* what() const noexcept override { return msg_.c_str(); }
  virtual ~AppException() noexcept = default;

private:
  std::string msg_;
};

// generic file error exception(filename)
class FileException : public AppException {
public:
  explicit FileException(const std::string &filename) : AppException(Format(filename)), filename_(filename) {}
  const std::string& filename() const noexcept { return filename_; }

private:
  std::string filename_;
  static std::string Format(const std::string &f);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Texture Loader Exceptions
//
// Texture loader exception: bad index
class TextureFileExceptionOutOfBounds : public FileException {
public:
  explicit TextureFileExceptionOutOfBounds(int index) : FileException(Format(index)) {}

private:
  static std::string Format(int idx);
};

// Texture loader exception: file is corrupted
class TextureFileExceptionCorrupted : public FileException {
public:
  explicit TextureFileExceptionCorrupted(const std::string &filename) : FileException(Format(filename)) {}

private:
  static std::string Format(const std::string &f);
};

// Texture loader exception: file is not a png
class TextureFileExceptionExtension : public FileException {
public:
  explicit TextureFileExceptionExtension(const std::string &filename) : FileException(Format(filename)) {}

private:
  static std::string Format(const std::string &f);
};

// Texture loader exception: texture array is empty
class TextureFileExceptionEmptyArray : public FileException {
public:
  explicit TextureFileExceptionEmptyArray(int index) : FileException(Format(index)) {}

private:
  static std::string Format(int index);
};

// Texture loader exception: no such file exists in the array
class TextureFileExceptionNoSuchFile : public FileException {
public:
  explicit TextureFileExceptionNoSuchFile(int index, const std::string& filename)
    : FileException(Format(index, filename)), _filename(filename) {}

private:
  std::string _filename;

  static std::string Format(int index, const std::string& filename);
};
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Labyrinth exceptions:
//
class LabExceptionCouldntMove : public AppException {
  public:
    explicit LabExceptionCouldntMove(int posX, int posY) : AppException(Format(posX, posY)) {}

  private:
    static std::string Format(int posX, int posY);
};