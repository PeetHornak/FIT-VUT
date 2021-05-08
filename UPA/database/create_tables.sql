CREATE TABLE asn (
    asn bigint PRIMARY KEY not null ,
    country varchar
);

CREATE TABLE speedtest (
    open_test_uuid uuid PRIMARY KEY NOT NULL ,
    download_kbit integer NOT NULL ,
    upload_kbit integer NOT NULL ,
    model varchar NOT NULL,
    network_type varchar NOT NULL,
    ping_ms real NOT NULL ,
    open_uuid uuid NOT NULL ,
    ip_anonym inet NOT NULL ,
    asn bigint REFERENCES asn
);


