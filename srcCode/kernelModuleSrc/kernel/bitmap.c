
// These functions are used in the kernel , sequentially AFTER memap initialization, which they rely on.





#define PAGE_SIZE 4096 
static uint8_t* pmm_bitmap;
static uint64_t pmm_total_pages;


// this function calculates the total pages of the final bitmap
// param regions is a pointer to the start of the regions array
// param regionCount is an unsigned integer for the total amount of expected safe-to-use regions
uint64_t count_Pages(memap_Region regions[], uint32_t regionCount)
{
    uint64_t pages = 0;

    for (uint32_t i = 0; i < regionCount; i++)
    {
        pages += regions[i].length / 4096; //bites of the region divided by bites of a page = pages per region
    }

    return pages;
}





void bitmap_Init(memap_Region regions[], uint32_t regionCount, uint64_t bitmapBase)
{
    pmm_total_pages = pmm_CountPages(regions, regionCount);

    uint64_t bitmapBytes = (pmm_total_pages + 7) / 8;

    bitmap = (uint8_t*)bitmapBase;

    //mark all pages used (1)
     for (uint64_t i = 0; i < bitmapBytes; i++)
    {
        bitmap[i] = 0xFF;
    }


}