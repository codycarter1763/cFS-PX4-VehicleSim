# This class can be used in your scripts like so:
#   load_utility 'CFS_Fleet/lib/cfs_fleet.py'
#   fleet = CFS_Fleet()
#   fleet.utility()
# For more information see the OpenC3 scripting guide

from openc3.script import *

class CFS_Fleet:
    def utility(self):
        pass
    def get_cfs_fleet_target_name_list(self):
        num_cfs_targets = <%= $cfs_fleet_globals_num_cfs_targets %>
        cfs_fleet_target_names = []
        for cpu_index in range(num_cfs_targets):
            cfs_target_name = f"CFS-{cpu_index + 1}"
            cfs_fleet_target_names.append(cfs_target_name)
        return cfs_fleet_target_names
