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
    case type::NEGATIVE_INTEGER:
    case type::POSITIVE_INTEGER:
        return QString::number(obj.as<long long>());
    case type::FLOAT:
        return QString::number(obj.as<double>());

    case type::STR:
    case type::BIN:
        return QString::fromStdString(obj.as<std::string>());

    default:
        qWarning() << "Type" << obj.type << "not implemented";
        return "";
    }
}

Block * parse_block(const msgpack::object_array & arr);

Sector * parse_struct(const msgpack::object_map & map)
{
    Sector * sector = new Sector();
    for (auto const& kv : map) {
        QString name = to_string(kv.key);
        Q_ASSERT(kv.val.type == type::ARRAY);

        Block * block = parse_block(kv.val.via.array);
        block->setName(name);
        sector->append(block);
    }

    return sector;
}

Block * parse_block(const msgpack::object_array & arr)
{
    Block * block = nullptr;

    int offset = arr.ptr[0].as<int>();
    int size = arr.ptr[1].as<int>();
    int t = arr.ptr[2].as<int>();

    auto & val = arr.ptr[3];
    Q_ASSERT(val.type != type::ARRAY);

    switch (val.type)
    {
    case type::MAP:
        block = parse_struct(val.via.map);
        break;
    default:
        block = new Block();
        QString valStr = to_string(val);
        block->setValueStr(valStr);
        break;
    }

    Q_ASSERT(block);

    block->setOffset(offset);
    block->setSize(size);

    return block;
}

Sector *read_structure(QString &filePath)
{
    qDebug() << "Load" << filePath;

    QProcess process;
    //process.start("python", {"D:\\Projects\\ReverseEngi\\ReverseEngiBeta\\interpreter\\parse_to_msgpack.py", "--in", filePath});
    process.start("python3", {"e:\\Projects\\ReverseEngiBeta\\interpreter\\parse_to_msgpack.py", "--in", filePath});
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
    //QByteArray out = QByteArray::fromHex("84aa646f735f68656164657294004000de0013a97369676e617475726594000200a24d5ab362797465735f696e5f6c6173745f626c6f636b94020200cc90ae626c6f636b735f696e5f66696c659404020003aa6e756d5f72656c6f63739406020000b16865616465725f706172616772617068739408020004b46d696e5f65787472615f70617261677261706873940a0200a430303030b46d61785f65787472615f70617261677261706873940c0200a446464646a27373940e0200a430303030a2737094100200a430304238a8636865636b73756d9412020000a269709414020000a263739416020000b272656c6f635f7461626c655f6f66667365749418020040ae6f7665726c61795f6e756d626572941a020000a9726573657276656431941c0800c4080000000000000000a66f656d5f69649424020000a86f656d5f696e666f9426020000a972657365727665643294281400c4140000000000000000000000000000000000000000a8655f6c66616e6577943c0400ccf0ab636f66665f68656164657294ccf0180088a97369676e617475726594ccf00400a450450000a76d616368696e6594ccf40200cd8664a873656374696f6e7394ccf6020006a86461746574696d6594ccf80400cb41d5f113bf800000a773796d5f74626c94ccfc040000a973796d5f636f756e7494cd0100040000a873697a655f6f707494cd01040200ccf0a8636861726163747394cd0106020022ad70655f6f70745f68656164657294cd0108ccf000de001ea56d6167696394cd01080200cd020bb46d616a6f725f6c696e6b65725f76657273696f6e94cd010a010009b46d696e6f725f6c696e6b65725f76657273696f6e94cd010b010000ac73697a655f6f665f636f646594cd010c0400ce000b8600b173697a655f6f665f696e69745f6461746194cd01100400ce0025c400b373697a655f6f665f756e696e69745f6461746194cd0114040000ab656e7472795f706f696e7494cd01180400ce0002b794ac626173655f6f665f636f646594cd011c0400a6307831303030aa696d6167655f6261736594cd01200800ab3078313030303030303030b173656374696f6e5f616c69676e6d656e7494cd01280400a6307831303030ae66696c655f616c69676e6d656e7494cd012c0400a53078323030ac6d616a6f725f6f735f76657294cd0130020006ac6d696e6f725f6f735f76657294cd0132020001af6d616a6f725f696d6167655f76657294cd0134020006af6d696e6f725f696d6167655f76657294cd0136020001ae6d616a6f725f737562735f76657294cd0138020006ae6d696e6f725f737562735f76657294cd013a020001ad77696e33325f76657273696f6e94cd013c040000aa696d6167655f73697a6594cd01400400ce00319000ac686561646572735f73697a6594cd01440400cd0400a8636865636b73756d94cd01480400ce00314900a973756273797374656d94cd014c020002a9646c6c5f636861727394cd014e0200cd8140b3737461636b655f726573657276655f73697a6594cd01500800ce00080000b2737461636b655f636f6d6d69745f73697a6594cd01580800cde000b1686561705f726573657276655f73697a6594cd01600800ce00100000b0686561705f636f6d6d69745f73697a6594cd01680800cd1000ac6c6f616465725f666c61677394cd0170040000a97276615f636f756e7494cd0174040010a8646174615f64697294cd0178cc8000de0010ab646174615f6469725b305d94cd0178080082a46164647294cd0178040000a473697a6594cd017c040000ab646174615f6469725b315d94cd0180080082a46164647294cd01800400ce000df198a473697a6594cd01840400cd0190ab646174615f6469725b325d94cd0188080082a46164647294cd01880400ce000fc000a473697a6594cd018c0400ce002196b8ab646174615f6469725b335d94cd0190080082a46164647294cd01900400ce000ef000a473697a6594cd01940400cdce70ab646174615f6469725b345d94cd0198080082a46164647294cd0198040000a473697a6594cd019c040000ab646174615f6469725b355d94cd01a0080082a46164647294cd01a00400ce00316000a473697a6594cd01a40400cd2664ab646174615f6469725b365d94cd01a8080082a46164647294cd01a80400ce000b944ca473697a6594cd01ac040038ab646174615f6469725b375d94cd01b0080082a46164647294cd01b0040000a473697a6594cd01b4040000ab646174615f6469725b385d94cd01b8080082a46164647294cd01b8040000a473697a6594cd01bc040000ab646174615f6469725b395d94cd01c0080082a46164647294cd01c0040000a473697a6594cd01c4040000ac646174615f6469725b31305d94cd01c8080082a46164647294cd01c8040000a473697a6594cd01cc040000ac646174615f6469725b31315d94cd01d0080082a46164647294cd01d0040000a473697a6594cd01d4040000ac646174615f6469725b31325d94cd01d8080082a46164647294cd01d80400ce000ba000a473697a6594cd01dc0400cd1b30ac646174615f6469725b31335d94cd01e0080082a46164647294cd01e00400ce000ded20a473697a6594cd01e40400cd0140ac646174615f6469725b31345d94cd01e8080082a46164647294cd01e8040000a473697a6594cd01ec040000ac646174615f6469725b31355d94cd01f0080082a46164647294cd01f0040000a473697a6594cd01f4040000a873656374696f6e7394cd01f8ccf00086ab73656374696f6e735b305d94cd01f828008aa46e616d6594cd01f80800a82e74657874000000a46d69736394cd02000400ce000b84adac7669727475616c5f6164647294cd02040400a6307831303030ab73697a655f6f665f72617794cd02080400a730786238363030ae706f696e7465725f746f5f72617794cd020c0400a53078343030b0706f696e7465725f746f5f72656c6f6394cd0210040000b2706f696e7465725f746f5f6c696e656e756d94cd0214040000a96e756d5f72656c6f6394cd0218020000ac6e756d5f6c696e656d6e756d94cd021a020000a8636861726163747394cd021c0400ce60000020ab73656374696f6e735b315d94cd022028008aa46e616d6594cd02200800a82e72646174610000a46d69736394cd02280400ce0002f08cac7669727475616c5f6164647294cd022c0400a730786261303030ab73697a655f6f665f72617794cd02300400a730783266323030ae706f696e7465725f746f5f72617794cd02340400a730786238613030b0706f696e7465725f746f5f72656c6f6394cd0238040000b2706f696e7465725f746f5f6c696e656e756d94cd023c040000a96e756d5f72656c6f6394cd0240020000ac6e756d5f6c696e656d6e756d94cd0242020000a8636861726163747394cd02440400ce40000040ab73656374696f6e735b325d94cd024828008aa46e616d6594cd02480800a82e64617461000000a46d69736394cd02500400cd40f4ac7669727475616c5f6164647294cd02540400a730786561303030ab73697a655f6f665f72617794cd02580400a6307833633030ae706f696e7465725f746f5f72617794cd025c0400a730786537633030b0706f696e7465725f746f5f72656c6f6394cd0260040000b2706f696e7465725f746f5f6c696e656e756d94cd0264040000a96e756d5f72656c6f6394cd0268020000ac6e756d5f6c696e656d6e756d94cd026a020000a8636861726163747394cd026c0400cec0000040ab73656374696f6e735b335d94cd027028008aa46e616d6594cd02700800a82e70646174610000a46d69736394cd02780400cdce70ac7669727475616c5f6164647294cd027c0400a730786566303030ab73697a655f6f665f72617794cd02800400a6307864303030ae706f696e7465725f746f5f72617794cd02840400a730786562383030b0706f696e7465725f746f5f72656c6f6394cd0288040000b2706f696e7465725f746f5f6c696e656e756d94cd028c040000a96e756d5f72656c6f6394cd0290020000ac6e756d5f6c696e656d6e756d94cd0292020000a8636861726163747394cd02940400ce40000040ab73656374696f6e735b345d94cd029828008aa46e616d6594cd02980800a82e72737263000000a46d69736394cd02a00400ce002196b8ac7669727475616c5f6164647294cd02a40400a730786663303030ab73697a655f6f665f72617794cd02a80400a83078323139383030ae706f696e7465725f746f5f72617794cd02ac0400a730786638383030b0706f696e7465725f746f5f72656c6f6394cd02b0040000b2706f696e7465725f746f5f6c696e656e756d94cd02b4040000a96e756d5f72656c6f6394cd02b8020000ac6e756d5f6c696e656d6e756d94cd02ba020000a8636861726163747394cd02bc0400ce40000040ab73656374696f6e735b355d94cd02c028008aa46e616d6594cd02c00800a82e72656c6f630000a46d69736394cd02c80400cd2664ac7669727475616c5f6164647294cd02cc0400a83078333136303030ab73697a655f6f665f72617794cd02d00400a6307832383030ae706f696e7465725f746f5f72617794cd02d40400a83078333132303030b0706f696e7465725f746f5f72656c6f6394cd02d8040000b2706f696e7465725f746f5f6c696e656e756d94cd02dc040000a96e756d5f72656c6f6394cd02e0020000ac6e756d5f6c696e656d6e756d94cd02e2020000a8636861726163747394cd02e40400ce42000040");

    msgpack::object_handle oh = msgpack::unpack(out.data(), out.size());
    msgpack::object o = oh.get();

    Q_ASSERT(o.type == type::MAP);
    Sector * root = parse_struct(o.via.map);
    return root;
}
