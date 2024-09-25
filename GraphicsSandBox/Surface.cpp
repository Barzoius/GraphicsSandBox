#include "Surface.h"
#include "Window.h"

#include <cassert>
#include <algorithm>
#include <sstream>
#include <filesystem>


Surface::Surface(unsigned int width, unsigned int height) 
{
    HRESULT hr = scratch.Initialize2D(
        format,
        width, height, 1u, 1u
    );

    if (FAILED(hr))
    {
        throw Surface::Exception(__LINE__, __FILE__, "Failed to initialize ScratchImage", hr);
    }
}



void Surface::Clear(Color fillValue) noexcept
{
    const auto width = GetWidth();
    const auto height = GetHeight();
    auto& imgData = *scratch.GetImage(0, 0, 0);

    for (size_t y = 0u; y < height; y++)
    {
        auto rowStart = reinterpret_cast<Color*>(imgData.pixels + imgData.rowPitch * y);
        std::fill(rowStart, rowStart + imgData.width, fillValue);
    }
}

void Surface::PutPixel(unsigned int x, unsigned int y, Color c) noexcept
{
    assert( 0 <= x < GetWidth());
    assert(0 <= y < GetHeight());

    auto& imgData = *scratch.GetImage(0, 0, 0);
    reinterpret_cast<Color*>(&imgData.pixels[y * imgData.rowPitch])[x] = c;
}

Surface::Color Surface::GetPixel(unsigned int x, unsigned int y) const noexcept
{
    assert(0 <= x < GetWidth());
    assert(0 <= y < GetHeight());

    auto& imgData = *scratch.GetImage(0, 0, 0);
    return reinterpret_cast<Color*>(&imgData.pixels[y * imgData.rowPitch])[x];
}

unsigned int Surface::GetWidth() const noexcept
{
    return (unsigned int)scratch.GetMetadata().width;
}


unsigned int Surface::GetHeight() const noexcept
{
    return (unsigned int)scratch.GetMetadata().height;
}

const Surface::Color* Surface::GetBufferPtr() const noexcept
{
    return const_cast<Surface*>(this)->GetBufferPtr();
}


Surface::Color* Surface::GetBufferPtr() noexcept
{
    return reinterpret_cast<Color*>(scratch.GetPixels());
}

const Surface::Color* Surface::GetBufferPtrConst() const noexcept
{
    return const_cast<Surface*>(this)->GetBufferPtr();
}

Surface Surface::FromFile(const std::string& sourceFile)
{

    wchar_t wideName[512];
    mbstowcs_s(nullptr, wideName, sourceFile.c_str(), _TRUNCATE);

    DirectX::ScratchImage scratch;
    
    HRESULT hr = DirectX::LoadFromWICFile(wideName, DirectX::WIC_FLAGS_NONE, nullptr, scratch);
    
    if (FAILED(hr))
    {
        throw Surface::Exception(__LINE__, __FILE__, sourceFile, "Failed to load image", hr);
    }


    if (scratch.GetImage(0, 0, 0)->format != format)
    {
        DirectX::ScratchImage converted;
        hr = DirectX::Convert(
            *scratch.GetImage(0, 0, 0),
            format,
            DirectX::TEX_FILTER_DEFAULT,
            DirectX::TEX_THRESHOLD_DEFAULT,
            converted
        );

        if (FAILED(hr))
        {
            throw Surface::Exception(__LINE__, __FILE__, sourceFile, "Failed to convert image", hr);
        }

        return Surface(std::move(converted));
    }

    return Surface(std::move(scratch));
}


void Surface::Save(const std::string& fileName) const
{
    
    const auto GetCodecID = [](const std::string& filename) {
        const std::filesystem::path path = filename;
        const auto ext = path.extension().string();


        if (ext == ".png")
        {
            return DirectX::WIC_CODEC_PNG;
        }
        else if (ext == ".jpg")
        {
            return DirectX::WIC_CODEC_JPEG;
        }
        else if (ext == ".bmp")
        {
            return DirectX::WIC_CODEC_BMP;
        }
        throw Exception(__LINE__, __FILE__, filename, "Image format not supported");
        };

    wchar_t wideName[512];
    mbstowcs_s(nullptr, wideName, fileName.c_str(), _TRUNCATE);

    HRESULT hr = DirectX::SaveToWICFile(
        *scratch.GetImage(0, 0, 0),
        DirectX::WIC_FLAGS_NONE,
        GetWICCodec(GetCodecID(fileName)),
        wideName
    );

    if (FAILED(hr))
    {
        throw Surface::Exception(__LINE__, __FILE__, fileName, "Failed to save image", hr);
    }

}

    
Surface::Surface(DirectX::ScratchImage scratch) noexcept : scratch(std::move(scratch))
{}

Surface::Exception::Exception(int line, const char* file, std::string note, std::optional<HRESULT> hr) noexcept
    :
    ModException(line, file),
    note("[Note] " + note)
{
    if (hr)
    {
        note = "[Error String] " + Window::Exception::TranslateErrorCode(*hr) + "\n" + note;
    }
}


Surface::Exception::Exception(int line, const char* file, std::string filename, std::string note_in, std::optional<HRESULT> hr) noexcept
    :
    ModException(line, file)
{
    using namespace std::string_literals;
    note = "[File] "s + filename + "\n"s + "[Note] "s + note_in;

    if (hr)
    {
        note = "[Error String] " + Window::Exception::TranslateErrorCode(*hr) + note;
    }
}


const char* Surface::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << ModException::what() << std::endl << GetNote();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* Surface::Exception::GetType() const noexcept
{
     return "Surface Exception";
}

const std::string& Surface::Exception::GetNote() const noexcept
{
    return note;
}

#include <random>

//Surface Surface::CreateHeightMap(unsigned int width, unsigned int height)
//{
//    std::unique_ptr<Color[]> pBuffer = nullptr;
//    pBuffer = std::make_unique<Color[]>(width * height);
//
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_int_distribution<> dis(0, 255);
//
//    
//    for (unsigned int i = 0; i < width * height; ++i)
//    {
//        int grayValue = dis(gen);
//        pBuffer[i] = Color(grayValue, grayValue, grayValue); 
//    }
//
//    Surface S =  Surface(width, height, std::move(pBuffer));
//    S.Save("Resources\\HeightMaps\\IMG??.bmp");
//    return S;
//}


bool Surface::AlphaLoaded() const noexcept
{
    return !scratch.IsAlphaAllOpaque();;
}