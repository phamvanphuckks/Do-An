//#include "sd_card.h"

/*
    res = f_mount(&fs, "", 1);

    if (res == FR_OK) {
        strcpy(buff, "/");
//    strcpy(buff, "//test");
        res = scan_files(buff);
    }

*/

//FRESULT showlist(char* path)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//    static FILINFO fno;

//    res = f_opendir(&dir, path);                       /* Open the directory */
//    printf("path %s\n", path);
//    if (res == FR_OK) {
//        for (;;) {
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);  
//                printf("%s%s - 1\n", path, fno.fname);
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//                printf("%s%s - 2\n", path, fno.fname);
//            }

//        }
//        f_closedir(&dir);
//    }

//    return res;
//}

//FRESULT scan_files (
//    char* path        /* Start node to be scanned (***also used as work area***) */
//)
//{
//    FRESULT res;
//    DIR dir;
//    UINT i;
//    static FILINFO fno;

//    res = f_opendir(&dir, path);                       /* Open the directory */
//    printf("path %s\n", path);
//    if (res == FR_OK) {
//        for (;;) {
//            res = f_readdir(&dir, &fno);                   /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
//            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
//                i = strlen(path);
//                sprintf(&path[i], "/%s", fno.fname);
//                
//                printf("%s%s - 1\n", path, fno.fname);
//                
//                res = scan_files(path);                    /* Enter the directory */
//                if (res != FR_OK) break;
//                path[i] = 0;
//            } else {                                       /* It is a file. */
//                printf("%s%s - 2\n", path, fno.fname);
//            }

//        }
//        f_closedir(&dir);
//    }

//    return res;
//}