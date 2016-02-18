#include "sai_redis.h"

/**
 *   Routine Description:
 *    @brief Internal set attribute
 *
 *  Arguments:
 *  @param[in] object_type - type of object
 *  @param[in] serialized_object_id - serialized object id
 *  @param[in] attr - attribute to serialize
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t internal_redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const std::string &serialized_object_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    if (attr_list == NULL)
    {
        REDIS_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }

    // TODO get is different, must be a producer and a consumer and blocking api
    // we need to serialize, push like create and then get
    


    REDIS_LOG_EXIT();

    return SAI_STATUS_NOT_IMPLEMENTED;
    //return SAI_STATUS_SUCCESS;
}

/**
 * Routine Description:
 *   @brief Generic get attribute
 *
 * Arguments:
 *    @param[in] object_type - the object type
 *    @param[in] object_id - the object id
 *    @param[in] attr_count - number of attributes
 *    @param[out] attr_list - array of attributes
 *
 * Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ sai_object_id_t object_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_object_id;
    sai_serialize_primitive(object_id, str_object_id);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_object_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_fdb_entry;
    sai_serialize_primitive(*fdb_entry, str_fdb_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_fdb_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_neighbor_entry;
    sai_serialize_primitive(*neighbor_entry, str_neighbor_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_neighbor_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_route_entry;
    sai_serialize_primitive(*unicast_route_entry, str_route_entry);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_route_entry,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_get_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id,
        _In_ uint32_t attr_count,
        _Out_ sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_vlan_id;
    sai_serialize_primitive(vlan_id, str_vlan_id);

    sai_status_t status = internal_redis_generic_get(
            object_type,
            str_vlan_id,
            attr_count,
            attr_list);

    REDIS_LOG_EXIT();

    return status;
}
