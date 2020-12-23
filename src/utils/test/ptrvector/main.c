/*
 * This file is part of SubFX,
 * Copyright (c) 2020 fdar0536
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>

#include "SubFX.h"

typedef struct Testing
{
    int a;
    int b;
} Testing;

Testing *createTesting()
{
    Testing *ret = calloc(1, sizeof(Testing));
    if (!ret)
    {
        return NULL;
    }

    ret->a = 0;
    ret->b = 0;

    return ret;
}

void freeTesting(void *in)
{
    if (!in)
    {
        return;
    }

    Testing *testing = (Testing *)in;
    free(testing);
}

void *deepCopyTesting(void *in)
{
    if (!in)
    {
        return NULL;
    }

    Testing *res = calloc(1, sizeof(Testing));
    if (!res)
    {
        return NULL;
    }

    Testing *input = (Testing *)in;
    res->a = input->a;
    res->b = input->b;
    return res;
}

subfx_exitstate dumpData(subfx_ptrVector *vecApi, subfx_handle vec)
{
    size_t size = 0;
    if (vecApi->size(vec, &size) == subfx_failed)
    {
        fputs("Fail to get size.", stderr);
        return subfx_failed;
    }

    size_t i = 0;
    Testing *data = NULL;
    for (i = 0; i < size; ++i)
    {
        data = vecApi->at(vec, i);
        if (!data)
        {
            fputs("Fail to get value.", stderr);
            return subfx_failed;
        }

        printf("data->a = %d, data->b = %d\n", data->a, data->b);
    }

    printf("\n");
    return subfx_success;
}

int main()
{
    SubFX *api = SubFX_init();
    if (!api)
    {
        fputs("Fail to create api entry.\n", stderr);
        return 1;
    }

    subfx_ptrVector *vecApi = api->ptrVector;

    subfx_handle vec = vecApi->create(freeTesting);
    if (!vec)
    {
        fputs("Fail to create vector.\n", stderr);
        SubFX_destroy(api);
        return 1;
    }

    if (vecApi->reserve(vec, 3) == subfx_failed)
    {
        fputs("Fail to reserve.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    };

    Testing *data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (vecApi->pushback(vec, data) == subfx_failed)
    {
        fputs("Fail to pushback.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data->a = 1;
    data->b = 2;
    if (vecApi->pushback(vec, data) == subfx_failed)
    {
        fputs("Fail to pushback.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data->a = 2;
    data->b = 3;
    if (vecApi->pushback(vec, data) == subfx_failed)
    {
        fputs("Fail to pushback.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (dumpData(vecApi, vec) == subfx_failed)
    {
        fputs("Fail to pushback.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data->a = 5;
    data->b = 5;
    if (vecApi->setValue(vec, 0, data) == subfx_failed)
    {
        fputs("Fail to set value.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (dumpData(vecApi, vec) == subfx_failed)
    {
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        return 1;
    }

    data = createTesting();
    if (!data)
    {
        fputs("Fail to allocate memory.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    data->a = 1;
    data->b = 1;
    if (vecApi->resize(vec, 5, data, deepCopyTesting) == subfx_failed)
    {
        fputs("Fail to resize.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    free(data);
    data = NULL;

    if (dumpData(vecApi, vec) == subfx_failed)
    {
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    size_t size = 0;
    size_t capacity = 0;
    if (vecApi->size(vec, &size) == subfx_failed)
    {
        fputs("Fail to get size.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (vecApi->capacity(vec, &capacity) == subfx_failed)
    {
        fputs("Fail to get capacity.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    printf("Before clear: size = %lld, capacity = %lld\n", size, capacity);

    if (vecApi->clear(vec) == subfx_failed)
    {
        fputs("Fail to clear.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (vecApi->size(vec, &size) == subfx_failed)
    {
        fputs("Fail to get size.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    if (vecApi->capacity(vec, &capacity) == subfx_failed)
    {
        fputs("Fail to get capacity.", stderr);
        if (api->closeHandle(vec) == subfx_failed)
        {
            fputs("Fail to close handle.\n", stderr);
        }

        SubFX_destroy(api);
        return 1;
    }

    printf("After clear: size = %lld, capacity = %lld\n", size, capacity);

    if (api->closeHandle(vec) == subfx_failed)
    {
        fputs("Fail to close handle.\n", stderr);

        SubFX_destroy(api);
        return 1;
    }

    SubFX_destroy(api);

    return 0;
}
