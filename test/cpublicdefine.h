#ifndef CPUBLICDEFINE_H
#define CPUBLICDEFINE_H
#include <QUuid>
#include <QVariant>
#include <QDateTime>

//图像
#define C_SOURCE_TYPE_NAME_SINGLE_IMAGE_SOURCE "lj_single_image_source"
#define C_SOURCE_TYPE_NAME_IMAGE_SOURCE "lj_image_source"

//白板
#define C_SOURCE_TYPE_NAME_WHITEBOARD "lj_source_whiteboard"

//色源
#define C_SOURCE_TYPE_NAME_COLOR_SOURCE "color_source"
//图像幻灯片放映
#define C_SOURCE_TYPE_NAME_SLIDESHOW "slideshow"
//媒体源
#define C_SOURCE_TYPE_NAME_FFMPEG_SOURCE "ffmpeg_source"
//文本 (GDI+)
#define C_SOURCE_TYPE_NAME_TEXT_GDIPLUS "text_gdiplus"
//文本 (FreeType 2)
#define C_SOURCE_TYPE_NAME_TEXT_FT2_SOURCE "text_ft2_source"

//窗口捕获
#define C_SOURCE_TYPE_NAME_WINDOW_CAPTURE "window_capture"
//游戏捕获
#define C_SOURCE_TYPE_NAME_GAME_CAPTURE "game_capture"

#ifdef __APPLE__
#define C_SOURCE_TYPE_NAME_INPUT_AUDIO_SOURCE  "coreaudio_input_capture"
#define C_SOURCE_TYPE_NAME_OUTPUT_AUDIO_SOURCE "coreaudio_output_capture"
//视频源捕获
#define C_SOURCE_TYPE_NAME_VIDEO_SOURCE "av_capture_input"
//显示器捕获
#define C_SOURCE_TYPE_NAME_MONITOR_CAPTURE "display_capture"
#elif _WIN32
#define C_SOURCE_TYPE_NAME_INPUT_AUDIO_SOURCE  "wasapi_input_capture"
#define C_SOURCE_TYPE_NAME_OUTPUT_AUDIO_SOURCE "wasapi_output_capture"
//视频源捕获
#define C_SOURCE_TYPE_NAME_VIDEO_SOURCE "dshow_input"
//显示器捕获
#define C_SOURCE_TYPE_NAME_MONITOR_CAPTURE "monitor_capture"
#else
#define C_SOURCE_TYPE_NAME_INPUT_AUDIO_SOURCE  "pulse_input_capture"
#define C_SOURCE_TYPE_NAME_OUTPUT_AUDIO_SOURCE "pulse_output_capture"
#endif

//#define C_SOURCE_TYPE_NAME_CLOUD_DOCUMENT "cloud_document"
#define C_SOURCE_TYPE_NAME_CLOUD_DOCUMENT "lj_clound_source"

#define C_DATA_NAME_FILE_LIST "c_data_name_file_list"
#define C_DATA_NAME_BLOCK_SIZE "c_data_name_block_size"
#define C_DATA_NAME_EDIT_STATUS "c_data_name_edit_status"
#define C_DATA_NAME_CLOUD_DOC_ID "c_data_name_cloud_doc_id"
#define C_DATA_NAME_CLOUD_DOC_NAME "c_data_name_cloud_doc_name"
#define C_DATA_NAME_CLOUD_DOC_CURRENT_PAGE "c_data_name_cloud_doc_current_page"
#define C_DATA_NAME_CLOUD_DOC_TRANSFERED "c_data_name_cloud_doc_transfered"
#define C_DATA_NAME_SCENE_LOCKED "c_data_name_scene_locked"
#define C_DATA_NAME_SCENE_LOCKED_BEFORE_STREAMING "c_data_name_scene_locked_before_streaming"
#define C_DATA_NAME_SCENE_ITEM_LOCKED_BEFORE_STREAMING "c_data_name_scene_item_locked_before_streaming"

static const int C_AUDIO_SOURCE_CHANNEL_FIRST_OUTPUT = 1;
static const int C_AUDIO_SOURCE_CHANNEL_FIRST_INPUT = 3;
static const int C_SESSION_NAME_MAX_LENGTH = 36;

enum HttpRequestType
{
    TYPE_REQUEST_UNDEFINED,
    TYPE_REQUEST_UPLOAD,
    TYPE_REQUEST_UPLOAD_WYS,
    TYPE_REQUEST_DOWNLOAD,
    TYPE_REQUEST_CHECK,
};

namespace CPB
{

enum LJWBCallbackType
{
    CALLBACK_TYPE_UNDEFINED,
    CALLBACK_TYPE_UNDO,
    CALLBACK_TYPE_REDO,
    CALLBACK_TYPE_CLEAR,
};
enum LJWBMouseButtonType
{
    MOUSE_BUTTON_TYPE_UNDEFINED = -1,//为了兼容原来obs里obs-interaction.h定义的obs_mouse_button_type
    MOUSE_BUTTON_TYPE_LEFT,
    MOUSE_BUTTON_TYPE_MIDDLE,
    MOUSE_BUTTON_TYPE_RIGHT,
};

enum EditButtonType
{
    EDIT_TYPE_UNDEFINE,
    EDIT_TYPE_RECT,
    EDIT_TYPE_ELLIPSE,
    EDIT_TYPE_PEN,
    EDIT_TYPE_TEXT,
    EDIT_TYPE_ARROW,
    EDIT_TYPE_CLEAR,
    EDIT_TYPE_COLOR,
    EDIT_TYPE_LINE_WIDTH,
    EDIT_TYPE_LINE,
    EDIT_TYPE_POINT,
    EDIT_TYPE_MOVE,
    EDIT_TYPE_VISIABLE,
    EDIT_TYPE_CLEAR_DRAW_TYPE,
    EDIT_TYPE_PEN_ALPHA
};

enum ButtonType
{
    BUTTON_TYPE_OK,
    BUTTON_TYPE_CANCEL,
    BUTTON_TYPE_KNOW,
};

enum ResolutionType
{
    RESOLUTION_LD=1,
    RESOLUTION_SD,
    RESOLUTION_HD,
};

enum RequestState
{
    REQUEST_STATE_NORMAL,
    REQUEST_STATE_DOING,
    REQUEST_STATE_SUCCESS,
    REQUEST_STATE_FAIL,
};

enum TransferState
{
    TRANSFER_STATE_NORMAL,
    TRANSFER_STATE_DOING,
    TRANSFER_STATE_CANCEL,
    TRANSFER_STATE_FAIL,
    TRANSFER_STATE_SUCCESS,
};

enum LoginErrorType
{
    LOGIN_ERROR_TYPE_NOERROR,
    LOGIN_ERROR_TYPE_PASSWORD,
    LOGIN_ERROR_TYPE_ROOMID,
    LOGIN_ERROR_TYPE_OTHER,
};

enum CommandType{
    COMMAND_TYPE_SIGN = 0,
    COMMAND_TYPE_INVESTIGATE,
    COMMAND_TYPE_EXAM,
    COMMAND_TYPE_ENROLL,
    COMMAND_TYPE_COURSE_LEARNING,
    COMMAND_TYPE_OTHER,
};

enum LotteryType{
    LOTTERY_TYPE_UNDEFINED = -1,
    LOTTERY_TYPE_SPECIAL = 0,
    LOTTERY_TYPE_FIRST,
    LOTTERY_TYPE_SECOND,
    LOTTERY_TYPE_THIRD,
    LOTTERY_TYPE_OTHER,
};

struct CommandData
{
    int type;
    QString content;
    QString link;
    QString name;
    QString error;
    CommandData()
        :type(COMMAND_TYPE_SIGN){}
};

struct RequestReplyData
{
    RequestReplyData()
        :type(0)
        ,networkErrorCode(-1)
        ,statusCode(-1)
        ,errorValue("")
        ,uuid(QUuid())
        ,state(REQUEST_STATE_NORMAL)
        ,startTime(QDateTime::currentDateTime())
        ,endTime(QDateTime::currentDateTime()){}
    int type;
    int networkErrorCode;
    int statusCode;
    QVariant errorValue;
    QVariant customValue;
    QUuid uuid;
    RequestState state;
    QDateTime startTime;
    QDateTime endTime;
};

struct RequestReplyResultData
{
    RequestReplyResultData()
        :hasError(false)
        ,type(0){}
    RequestReplyResultData(bool _hasError,int _type,const QVariant &_value)
        :hasError(_hasError)
        ,type(_type)
        ,value(_value){}
    bool hasError;
    int type;
    QVariant value;
};

struct DataUpdateParam
{
    DataUpdateParam()
        :type(0)
        ,id(0)
        ,orgId(0)
        ,value(""){}
    int type;
    quint64 id;
    quint64 orgId;
    QVariant value;
};

struct LoginParam
{
    QString userId;
    QString pwd;
    QString userName;
    QString roomId;
    QString avatar;
    QString sign;
    QString sessionName;
    QString sReserved1;
    QString sReserved2;
    QString sReserved3;
};

struct ErrorData
{
    QString description(){return QString("key=%1,message=%2").arg(key).arg(message);}
    QString key;
    QString message;
};

struct MultimediaData
{
    MultimediaData()
        :audioStreamIndex(-1),videoStreamIndex(-1)
        ,width(0),height(0),hasError(true),onlyAudio(false){}
    int audioStreamIndex;
    int videoStreamIndex;
    int width;
    int height;
    bool hasError;
    bool onlyAudio;
    QString errorString;
    QVariant value;
};

struct SecondTimerData{
    bool started;
    int currentSecond;
    int interval;
    int type;
    SecondTimerData(int _interval,int _type):started(false),currentSecond(0),interval(_interval),type(_type){};
    void start(){
       started = true;
       currentSecond = 0;
    };
    void stop()
    {
        started = false;
        currentSecond = 0;
    }
};
}

Q_DECLARE_METATYPE(CPB::CommandType)
Q_DECLARE_METATYPE(CPB::CommandData)
Q_DECLARE_METATYPE(CPB::TransferState)
Q_DECLARE_METATYPE(CPB::EditButtonType)

#endif // CPUBLICDEFINE_H
