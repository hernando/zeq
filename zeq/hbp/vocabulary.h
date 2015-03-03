
/* Copyright (c) 2014-2015, Human Brain Project
 *                          Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 *                          Juan Hernando <jhernando@fi.upm.es>
 *                          Grigori Chevtchenko <grigori.chevtchenko@epfl.ch>
 */

#ifndef ZEQ_HBP_VOCABULARY_H
#define ZEQ_HBP_VOCABULARY_H

#include <zeq/types.h>
#include <zeq/api.h>

namespace zeq
{
namespace hbp
{
namespace data
{

/**
 * This structure holds informations about JPEG images.
 * It has the size infomations and the image data, data is a pointer
 * to avoid copy. The data pointer is only valid as long as the zeq Event
 * or rendering resource internal event with image data stays alive.
 */
struct ImageJPEG
{
    ImageJPEG( const uint32_t sizeInBytes, const uint8_t* data )
        : _sizeInBytes( sizeInBytes )
        , _data( data )
    {}
    uint32_t getSizeInBytes() const { return _sizeInBytes; }
    const uint8_t* getDataPtr() const { return _data; }

private:
    const uint32_t _sizeInBytes;
    const uint8_t* _data;
};

}

static const uint128_t EVENT_HEARTBEAT(
    lunchbox::make_uint128( "hbp::HeartbeatEvent" ));
/** @group HBP messages */
//@{
static const uint128_t EVENT_SELECTED_IDS(
    lunchbox::make_uint128( "hbp::SelectedIDsEvent" ));
static const uint128_t EVENT_TOGGLE_ID_REQUEST(
    lunchbox::make_uint128( "hbp::ToggleIDRequest" ));
//@}

/**
 * Serialize the given JPEG image into an Event of type EVENT_IMAGEJPEG.
 * @param image the JPEG image.
 * @return the serialized event.
 */
::zeq::Event serializeImageJPEG( const data::ImageJPEG& image );

/**
 * Deserialize the given EVENT_IMAGEJPEG event into an JPEG image.
 * @param event the zeq EVENT_IMAGEJPEG.
 * @return the jpeg image.
 */
data::ImageJPEG deserializeImageJPEG( const ::zeq::Event& event );

/**
 * Serialize the given event type into an Event of type EVENT_REQUEST.
 * Based on the type, the target application is responsible to send back
 * the requested event.
 * @param type the type of event that the application should send back.
 * @return the serialized event.
 */
::zeq::Event serializeRequest( const lunchbox::uint128_t& eventType );

/**
 * Deserialize the given request event into a uint128_t.
 * The uint128_t received is a zeq event.
 * @param event the zeq EVENT_REQUEST.
 * @return an uint128_t to identify the zeq event to be created.
 */
lunchbox::uint128_t deserializeRequest( const ::zeq::Event& event );

/**
 * Serialize the given camera matrix into an Event of type EVENT_CAMERA.
 * The matrix to be serialized and sent to other instances is the one
 * used in the application to transform from world coordinate space into
 * camera space. Microns are assumed as unit and it has the BBP circuit
 * as reference.
 * @param matrix the 4x4 camera matrix in OpenGL data layout.
 * @return the serialized event.
 */
ZEQ_API Event serializeCamera( const std::vector< float >& matrix );

/**
 * Deserialize the given camera event into the 4x4 matrix.
 * The matrix received and deserialized is the one used in the application
 * to transform from world coordinate space into camera space.
 * Microns are assumed as unit and it has the BBP circuit as reference.
 * @param camera the camera event generated by serializeCamera().
 * @return the 4x4 camera matrix in OpenGL data layout.
 */
ZEQ_API std::vector< float > deserializeCamera( const Event& event );

/**
 * Serialize the given neuron selection into an Event of type
 * EVENT_SELECTED_IDS.
 * @param selection vector of neuron GIDs (uint).
 * @return the serialized event.
 */
ZEQ_API
Event serializeSelectedIDs( const std::vector< unsigned int >& ids );

/**
 * Deserialize the given neuron selection event into the vector of neuron GIDs.
 * @param event a selection event generated by serializeSelection().
 * @return the vector of neuron GIDs (uint).
 */
ZEQ_API
std::vector< unsigned int > deserializeSelectedIDs( const Event& event );

/**
 * Serialize the given selection into an Event of type EVENT_TOGGLE_ID_REQUEST.
 * @param ids vector of neuron GIDs (uint).
 * @return the serialized event.
 */
ZEQ_API
Event serializeToggleIDRequest( const std::vector< unsigned int >& ids );

/**
 * Deserialize an toggle selection request event into the vector of neuron GIDs.
 * @param event an event generated by serializeToggleIDRequest().
 * @return the vector of neuron GIDs (uint).
 */
ZEQ_API
std::vector< unsigned int > deserializeToggleIDRequest( const Event& event );

}
}
#endif
