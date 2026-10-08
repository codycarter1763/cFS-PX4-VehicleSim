############################################
#
# Adapter class allowing the unified test script
# to execute within COSMOS.  The cmd/tlm calls are
# mapped to the corresponding COSMOS API call.
#
############################################

from cfs_test_connection import CfsTest_Connection

import openc3.script as oc3
import re

class CfsTest_Cosmos_Adapter(CfsTest_Connection):

    @staticmethod
    def to_cosmos_mnemonic(id : str):
        tempstr = ''
        terms = list()

        # The intent here is to split at CamelCaseTerms, e.g.
        # 'TheLongAndWindingCommand' -> ['The', 'Long', 'And', 'Winding', 'Command']
        for word in re.findall(r'[A-Z][a-z_]*', id):
            # Now find and combine single letters in the terms
            if len(word) > 1:
                trimmed_word = word.rstrip('_')
                if len(trimmed_word) == 1:
                    tempstr += trimmed_word
                    trimmed_word = None
                if tempstr != '':
                    terms.append(tempstr)
                    tempstr = '' # start over
                if trimmed_word:
                    terms.append(trimmed_word)
            else:
                tempstr += word

        # add any final term
        if tempstr != '':
            terms.append(tempstr)

        return '_'.join(map(lambda s: s.upper(), terms))

    def print(self, msg : str):
        self.cosmos_obj.print(msg)
        return

    def wait_check_packet(self, tlm_id, num_pkt, timeout):
        tlm_spec = f"{self.get_app_name()}_{tlm_id}" # Convert to a COSMOS-style term
        return oc3.wait_check_packet(self.get_instance_name(), tlm_spec, num_pkt, timeout)

    def tlm(self, tlm_id, param_id):
        tlm_spec = f"{self.get_app_name()}_{tlm_id}" # Convert to a COSMOS-style term
        param_id = CfsTest_Cosmos_Adapter.to_cosmos_mnemonic(param_id) # Convert to a COSMOS-style term
        return oc3.tlm(self.get_instance_name(), tlm_spec, param_id)

    def cmd(self, cmd_id, **kwargs):
        cmd_id = CfsTest_Cosmos_Adapter.to_cosmos_mnemonic(cmd_id) # Convert to a COSMOS-style term
        cmd_spec = f"{self.get_app_name()}_CMD_{cmd_id}"
        cosmos_args = dict()
        for param,value in kwargs.items():
            cosmos_args[self.to_cosmos_mnemonic(param)] = value
        return oc3.cmd(self.get_instance_name(), cmd_spec, cosmos_args)

    def wait_check(self, tlm_id, param_id, value, timeout):
        tlm_spec = f"{self.get_app_name()}_{tlm_id}" # Convert to a COSMOS-style term
        param_id = CfsTest_Cosmos_Adapter.to_cosmos_mnemonic(param_id) # Convert to a COSMOS-style term
        expr = f"{self.get_instance_name()} {tlm_spec} {param_id} == {value}"
        return oc3.wait_check(expr, timeout)

    def spawn_child(self, app_name:str):
        return CosmosSecondary(self, app_name)

    def get_tlm_msg_id(self, tlm_id:str):
        result = None
        key_name = f"{self.get_app_name()}_{tlm_id}"
        result = self.cosmos_obj.tlm_mids[key_name]
        return result

    def get_cmd_msg_id(self):
        result = None
        key_name = self.get_app_name()
        result = self.cosmos_obj.cmd_mids[key_name]
        return result

    def wait(self, timeout):
        return oc3.wait(timeout)

    def execute(self, method : str):
        f = getattr(self.method_obj, method)
        return f()

    def __init__(self, cosmos_obj : oc3.suite.Group, method_type, **kwargs):
        CfsTest_Connection.__init__(self, method_type, **kwargs)
        self.cosmos_obj = cosmos_obj
        if 'tlm_mids' in kwargs:
            self.cosmos_obj.tlm_mids = kwargs['tlm_mids']
        if 'cmd_mids' in kwargs:
            self.cosmos_obj.cmd_mids = kwargs['cmd_mids']
        return

# A secondary shares the same astro instance and just talks to a different app
class CosmosSecondary(CfsTest_Cosmos_Adapter):
    def __init__(self, parent_obj : CfsTest_Cosmos_Adapter, app_name:str, **kwargs):
        CfsTest_Connection.__init__(self, None, app_name = app_name, instance_name = parent_obj.get_instance_name())
        self.cosmos_obj = parent_obj.cosmos_obj
