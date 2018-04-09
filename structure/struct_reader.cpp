#include "struct_reader.h"

#include <QFile>
#include <QProcess>

#include <msgpack.hpp>
#include <iostream>

#include "block.h"

using type = msgpack::type::object_type;

QString to_string(const msgpack::object & obj)
{
    switch (obj.type)
    {
    case type::ARRAY:
        return "[]";

    case type::NEGATIVE_INTEGER:
    case type::POSITIVE_INTEGER:
        return QString::number(obj.as<long long>());
    case type::FLOAT:
        return QString::number(obj.as<double>());

    case type::STR:
    case type::BIN:
        return QString::fromStdString(obj.as<std::string>());

    case type::MAP:
        //auto map = obj.as<std::multimap>();
        return "map";

    default:
        qWarning() << "Type" << obj.type << "not implemented";
        return "";
    }
}

Block * parse_block(const msgpack::object_kv & kv);
Block * parse_block(QString & name, const msgpack::object_map & map);

Sector * parse_struct(const msgpack::object_map & map)
{
    Sector * sector = new Sector();

    for (auto const& kv : map) {
        Block * block = parse_block(kv);
        sector->append(block);
    }

    return sector;
}


Sector * parse_array(const QString & parent_name, const msgpack::object_array & array)
{
    Sector * sector = new Sector();
    int i = 0;
    for (auto const& obj : array) {
        QString name = QString("%1[%2]").arg(parent_name).arg(i);
        Block * block = parse_block(name, obj.via.map);
        sector->append(block);
        ++i;
    }
    return sector;
}

Block * parse_block(const msgpack::object_kv & kv)
{
    QString name = QString::fromStdString(kv.key.as<std::string>());

    if (kv.val.type != type::MAP)
        qWarning() << name;
    Q_ASSERT(kv.val.type == type::MAP);

    return parse_block(name, kv.val.via.map);
}

Block * parse_block(QString & name, const msgpack::object_map & map)
{
    Block * block = nullptr;

    int offset = 0;
    int size = 0;

    for (auto const& val_kv : map) {
        auto key = val_kv.key.as<std::string>();
        auto & val = val_kv.val;

        if (key == "offset") {
            offset = val.as<int>();
        } else if (key == "size") {
            size = val.as<int>();
        } else if (key == "value") {
            switch (val.type)
            {
            case type::MAP:
                block = parse_struct(val.via.map);
                break;
            case type::ARRAY:
                if (val.via.array.size > 0 && val.via.array.ptr[0].type == type::MAP)
                    block = parse_array(name, val.via.array);
                else {
                    block = new Block();
                    QString valStr = to_string(val);
                    block->setValueStr(valStr);
                }
                break;
            default:
                block = new Block();
                QString valStr = to_string(val);
                block->setValueStr(valStr);
                break;
            }
        } else if (key == "format") {
            // TODO parse format
        } else {
            qDebug() << QString::fromStdString(key);
        }
    }

    Q_ASSERT(block);

    block->setName(name);
    block->setOffset(offset);
    block->setSize(size);

    return block;
}

Sector *read_structure(QString &filePath)
{
    qDebug() << "Load" << filePath;

    QProcess process;
    //process.start("python", {"D:\\Projects\\ReverseEngi\\ReverseEngiBeta\\interpreter\\parse_to_msgpack.py", "--in", filePath});
    process.start("\"c:\\Program Files (x86)\\Microsoft Visual Studio\\Shared\\Python36_64\\python.exe\"", {"e:\\Projects\\ReverseEngiBeta\\interpreter\\parse_to_msgpack.py", "--in", filePath});
    if (!process.waitForFinished())
    {
        qDebug() << process.errorString();
        return nullptr;
    }
    if (process.exitCode() != 0)
    {
        qDebug() << "Exit code:" << process.exitCode();
        qDebug() << process.errorString();
        qDebug() << process.readAll();
        return nullptr;
    }

    QByteArray out = process.readAllStandardOutput();
    //QByteArray out = QByteArray::fromHex("84aa646f735f68656164657283a576616c7565de0013a97369676e617475726584a576616c7565c4024d5aa66f666673657400a473697a6502a6666f726d6174a23273b362797465735f696e5f6c6173745f626c6f636b84a576616c7565cc90a66f666673657402a473697a6502a6666f726d6174a148ae626c6f636b735f696e5f66696c6584a576616c756503a66f666673657404a473697a6502a6666f726d6174a148aa6e756d5f72656c6f637384a576616c756500a66f666673657406a473697a6502a6666f726d6174a148b16865616465725f7061726167726170687384a576616c756504a66f666673657408a473697a6502a6666f726d6174a148b46d696e5f65787472615f7061726167726170687384a576616c7565a430303030a66f66667365740aa473697a6502a6666f726d6174a148b46d61785f65787472615f7061726167726170687384a576616c7565a446464646a66f66667365740ca473697a6502a6666f726d6174a148a2737384a576616c7565a430303030a66f66667365740ea473697a6502a6666f726d6174a148a2737084a576616c7565a430304238a66f666673657410a473697a6502a6666f726d6174a148a8636865636b73756d84a576616c756500a66f666673657412a473697a6502a6666f726d6174a148a2697084a576616c756500a66f666673657414a473697a6502a6666f726d6174a148a2637384a576616c756500a66f666673657416a473697a6502a6666f726d6174a148b272656c6f635f7461626c655f6f666673657484a576616c756540a66f666673657418a473697a6502a6666f726d6174a148ae6f7665726c61795f6e756d62657284a576616c756500a66f66667365741aa473697a6502a6666f726d6174a148a972657365727665643184a576616c75659400000000a66f66667365741ca473697a6508a6666f726d6174a23448a66f656d5f696484a576616c756500a66f666673657424a473697a6502a6666f726d6174a148a86f656d5f696e666f84a576616c756500a66f666673657426a473697a6502a6666f726d6174a148a972657365727665643284a576616c75659a00000000000000000000a66f666673657428a473697a6514a6666f726d6174a3313048a8655f6c66616e657784a576616c7565cce0a66f66667365743ca473697a6504a6666f726d6174a149a66f66667365741ca473697a6524ab636f66665f68656164657283a576616c756588a97369676e617475726584a576616c7565c40450450000a66f6666736574cce0a473697a6504a6666f726d6174a23473a76d616368696e6584a576616c7565cd8664a66f6666736574cce4a473697a6502a6666f726d6174a148a873656374696f6e7384a576616c756506a66f6666736574cce6a473697a6502a6666f726d6174a148a86461746574696d6584a576616c7565cb41d359cbb9000000a66f6666736574cce8a473697a6504a6666f726d6174a149a773796d5f74626c84a576616c756500a66f6666736574cceca473697a6504a6666f726d6174a149a973796d5f636f756e7484a576616c756500a66f6666736574ccf0a473697a6504a6666f726d6174a149a873697a655f6f707484a576616c7565ccf0a66f6666736574ccf4a473697a6502a6666f726d6174a148a8636861726163747384a576616c756522a66f6666736574ccf6a473697a6502a6666f726d6174a148a66f6666736574cce0a473697a6518ad70655f6f70745f68656164657283a576616c7565de0011a56d6167696384a576616c7565cd020ba66f666673657400a473697a6502a6666f726d6174a148b46d616a6f725f6c696e6b65725f76657273696f6e84a576616c756509a66f666673657402a473697a6501a6666f726d6174a142b46d696e6f725f6c696e6b65725f76657273696f6e84a576616c756500a66f666673657403a473697a6501a6666f726d6174a142ac73697a655f6f665f636f646584a576616c7565ce000b7c00a66f666673657404a473697a6504a6666f726d6174a149b173697a655f6f665f696e69745f6461746184a576616c7565ce00205600a66f666673657408a473697a6504a6666f726d6174a149b373697a655f6f665f756e696e69745f6461746184a576616c756500a66f66667365740ca473697a6504a6666f726d6174a149ab656e7472795f706f696e7484a576616c7565ce0002b754a66f666673657410a473697a6504a6666f726d6174a149ac626173655f6f665f636f646584a576616c7565a6307831303030a66f666673657414a473697a6504a6666f726d6174a149aa696d6167655f6261736584a576616c7565cf2e74657874000000a66f6666736574cd01e8a473697a6508a6666f726d6174a151b173656374696f6e5f616c69676e6d656e7484a576616c7565ceb97a0b00a66f6666736574cd01f0a473697a6504a6666f726d6174a149ae66696c655f616c69676e6d656e7484a576616c7565ce00100000a66f6666736574cd01f4a473697a6504a6666f726d6174a149ac6d616a6f725f6f735f76657284a576616c75657ca66f6666736574cd01f8a473697a6502a6666f726d6174a148ac6d696e6f725f6f735f76657284a576616c7565cd0b00a66f6666736574cd01faa473697a6502a6666f726d6174a148af6d616a6f725f696d6167655f76657284a576616c756506a66f6666736574cd01fca473697a6502a6666f726d6174a148af6d696e6f725f696d6167655f76657284a576616c756500a66f6666736574cd01fea473697a6502a6666f726d6174a148ae6d616a6f725f737562735f76657284a576616c756500a66f6666736574cd0200a473697a6502a6666f726d6174a148ae6d696e6f725f737562735f76657284a576616c756500a66f6666736574cd0202a473697a6502a6666f726d6174a148a66f6666736574cd01e8a473697a651ca672656c6f637383a576616c756590a66f666673657440a473697a6500");

    msgpack::object_handle oh = msgpack::unpack(out.data(), out.size());
    msgpack::object o = oh.get();

    Sector * root = parse_struct(o.via.map);
    return root;
}
