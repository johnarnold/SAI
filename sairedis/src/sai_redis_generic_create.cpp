#include "sai_redis.h"

uint64_t object_id_counter = 0;

sai_object_id_t redis_create_virtual_object_id(
        _In_ sai_object_type_t object_type)
{
    // when started, we need to get current status of
    // generated objects and pick up values from there
    // we should also keep mapping of those values in syncd
    //
    // since from object_id ws should be able to get object type
    // so set top 16 bits of object ad as object type
    // 2^48 should be enough to have objects per object type
    // also 2^16 should be enough to have different object types
    // in future we could change this to find "holes" after delted
    // objects, but can be tricky since this information would need
    // to be stored somewhere in case of oa restart
    //
    // TODO make this atomic

    uint64_t virtual_id = object_id_counter++;

    return (((sai_object_id_t)object_type) << 48) | virtual_id;
}

/**
 *   Routine Description:
 *    @brief Generic create method
 *
 *  Arguments:
 *  @param[out] acl_table_id - the the acl table id
 *  @param[in] attr_count - number of attributes
 *  @param[in] attr_list - array of attributes
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t internal_redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const std::string &serialized_object_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    if (attr_list == NULL)
    {
        REDIS_LOG_EXIT();
        return SAI_STATUS_INVALID_PARAMETER;
    }

    std::string str_object_type;
    sai_serialize_primitive(object_type, str_object_type);

    std::string str_common_api;
    sai_serialize_primitive(SAI_COMMON_API_CREATE, str_common_api);

    std::vector<ssw::FieldValueTuple> entry;

    for (uint32_t index = 0; index < attr_count; ++index)
    {
        const sai_attribute_t *attr = &attr_list[index];

        sai_attr_serialization_type_t serialization_type;

        sai_status_t status = sai_get_serialization_type(object_type, attr->id, serialization_type);

        if (status != SAI_STATUS_SUCCESS)
        {
            REDIS_LOG_ERR("Unable to find serialization type for object type: %u and attribute id: %u, status: %u",
                    object_type,
                    attr->id,
                    status);

            REDIS_LOG_EXIT();
            return status;
        }

        std::string str_attr_id;
        sai_serialize_attr_id(*attr, str_attr_id);

        std::string str_attr_value;
        status = sai_serialize_attr_value(serialization_type, *attr, str_attr_value);

        if (status != SAI_STATUS_SUCCESS)
        {
            REDIS_LOG_ERR("Unable to serialize attribute for object type: %u and attribute id: %u, status: %u",
                    object_type,
                    attr->id,
                    status);

            REDIS_LOG_EXIT();
            return status;
        }

        ssw::FieldValueTuple fvt(str_attr_id, str_attr_value);

        entry.push_back(fvt);
    }

    std::string key = str_object_type + ":" + serialized_object_id;

    g_asicState->set(key, entry, "create");

    REDIS_LOG_EXIT();

    return SAI_STATUS_SUCCESS;
}

/**
 *   Routine Description:
 *    @brief Generic create method
 *
 *  Arguments:
 *  @param[out] acl_table_id - the the acl table id
 *  @param[in] attr_count - number of attributes
 *  @param[in] attr_list - array of attributes
 *
 *  Return Values:
 *    @return  SAI_STATUS_SUCCESS on success
 *             Failure status code on error
 */
sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _Out_ sai_object_id_t* object_id,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    *object_id = redis_create_virtual_object_id(object_type);

    std::string str_object_id;
    sai_serialize_primitive(*object_id, str_object_id);

    sai_status_t status = internal_redis_generic_create(
                            object_type,
                            str_object_id,
                            attr_count,
                            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_fdb_entry_t *fdb_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    std::string str_fdb_entry;
    sai_serialize_primitive(*fdb_entry, str_fdb_entry);

    sai_status_t status = internal_redis_generic_create(
                            object_type,
                            str_fdb_entry,
                            attr_count,
                            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_neighbor_entry_t* neighbor_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    // rif_id and ip_address
    // rif_id must be valid virtual id
    std::string str_neighbor_entry;
    sai_serialize_primitive(*neighbor_entry, str_neighbor_entry);

    sai_status_t status = internal_redis_generic_create(
                            object_type,
                            str_neighbor_entry,
                            attr_count,
                            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create(
        _In_ sai_object_type_t object_type,
        _In_ const sai_unicast_route_entry_t* unicast_route_entry,
        _In_ uint32_t attr_count,
        _In_ const sai_attribute_t *attr_list)
{
    REDIS_LOG_ENTER();

    // vr_id and destintion
    // vr_id must be valid virtual router id
    std::string str_route_entry;
    sai_serialize_primitive(*unicast_route_entry, str_route_entry);

    sai_status_t status = internal_redis_generic_create(
                            object_type,
                            str_route_entry,
                            attr_count,
                            attr_list);

    REDIS_LOG_EXIT();

    return status;
}

sai_status_t redis_generic_create_vlan(
        _In_ sai_object_type_t object_type,
        _In_ sai_vlan_id_t vlan_id)
{
    REDIS_LOG_ENTER();

    std::string str_vlan_id;
    sai_serialize_primitive(vlan_id, str_vlan_id);

    // create_vlan have no attributes, but generic method needs one
    // so provide count as zero and dummy attribute
    sai_attribute_t dummy_attribute;

    sai_status_t status = internal_redis_generic_create(
                            object_type,
                            str_vlan_id,
                            0,
                            &dummy_attribute);

    REDIS_LOG_EXIT();

    return status;
}
