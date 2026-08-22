#pragma once

// NOTE: This gets moved from Engine to NetCore at some point, i don't really care but still
struct FFastArraySerializerItem
{
    int32 ReplicationID;
    int32 ReplicationKey;
    int32 MostRecentArrayReplicationKey;
};
