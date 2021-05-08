from dataclasses import dataclass, fields
from uuid import UUID


@dataclass
class Databases:
    asn: str = "asn"
    speedtest: str = "speedtest"


@dataclass(order=True)
class Speedtest:
    open_test_uuid: str
    open_uuid: str
    download_kbit: int
    upload_kbit: int
    ping_ms: float
    ip_anonym: str
    asn: int
    model: str
    network_type: str

    def __post_init__(self):
        is_ipv6 = ":" in self.ip_anonym
        self.open_uuid = self.open_uuid[1:]
        self.open_test_uuid = self.open_test_uuid[1:]
        self.ip_anonym = self.ip_anonym + "/80" if is_ipv6 else self.ip_anonym + "/24"
        self.asn = int(self.asn) if self.asn else self.asn
        self.download_kbit = int(self.download_kbit)
        self.upload_kbit = int(self.upload_kbit)
        validate_speedtest(self)


@dataclass(order=True)
class ASN:
    asn: int
    country: str

    def __post_init__(self):
        validate_asn(self)


def get_dataclass_field_names(dclass):
    return [field.name for field in fields(dclass)]


def validate_speedtest(instance):
    for field in fields(instance)[:5]:  # check only first 4 compulsory attrs
        attr = getattr(instance, field.name)

        if "uuid" in field.name:  # check valid UUID, throws value error
            UUID(attr, version=4)

        if not isinstance(attr, field.type):
            msg = "Field {0.name} is of type {1}, should be {0.type}".format(field, type(attr))
            raise ValueError(msg)


def validate_asn(instance):
    for field in fields(instance):
        attr = getattr(instance, field.name)
        if not isinstance(attr, field.type):
            msg = "Field {0.name} is of type {1}, should be {0.type}".format(field, type(attr))
            raise ValueError(msg)
