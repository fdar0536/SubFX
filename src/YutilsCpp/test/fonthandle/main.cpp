#include <cstdio>

#include "YutilsCpp"

int main()
{
    std::string tmpString("Source Code Pro");
    auto handle = Yutils::FontHandle::create(
        tmpString, // family
                0, // bold
                0, // italic
                0, // underline
                0, // strikeout
               60, // size
               1., // xscale
               1., // yscale
               0. // hspace
               );

    if (handle == nullptr)
    {
        puts("Fail in initializing.");
        return 1;
    }

    auto map = handle->metrics();
    if (!map.size())
    {
        puts("Fail to get font metrics");
        return 1;
    }

    printf("font's height: %lf\n", map["height"]);
    printf("font's ascent: %lf\n", map["ascent"]);
    printf("font's descent: %lf\n", map["descent"]);
    printf("font's internal leading: %lf\n",
           map["internal_leading"]);
    printf("font's external leading: %lf\n",
           map["external_leading"]);

    tmpString = "testing";
    map = handle->text_extents(tmpString);
    if (!map.size())
    {
        puts("Fail to get text extents");
        return 1;
    }

    printf("text's width is: %lf\n", map["width"]);
    printf("text's height is: %lf\n", map["height"]);

    tmpString = handle->text_to_shape(tmpString);
    if (!tmpString.length())
    {
        puts("Fail to get text's shape");
        return 1;
    }

    printf("text's shape is: %s\n", tmpString.c_str());
    return 0;
}
