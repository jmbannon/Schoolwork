#ifndef ERROR_H_
#define ERROR_H_

#define ERROR_RETURN(error_message, ret) printf("\n ERROR: %s\n", error_message); return (ret);
#define ERROR(error_message) ERROR_RETURN(error_message, 1);

#define CHECK_RETURN(error_condition, ret)\
if (error_condition) \
{\
    return (ret);\
}

#define CHECK_ERROR_RETURN(error_condition, error_message, ret)\
if (error_condition) \
{\
    ERROR_RETURN(error_message, ret);\
}

#define CHECK_ZERO_RETURN(res) CHECK_RETURN((res) != 0, res)
#define CHECK_ZERO_ERROR_RETURN(res, error_message) CHECK_ERROR_RETURN((res) != 0, error_message, res)

#define CHECK_NULL_RETURN(res) CHECK_RETURN((res) == NULL, NULL_FAIL)
#define CHECK_MALLOC_RETURN(res) CHECK_NULL_RETURN(res);

#define NULL_FAIL 1
#endif
