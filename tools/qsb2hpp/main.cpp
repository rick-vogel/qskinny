#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#if QT_VERSION >= QT_VERSION_CHECK( 6, 6, 0 )
#include <qfile.h>
#include <qiodevice.h>
#include <rhi/qshader.h>
#include <rhi/qshaderdescription.h>
#endif

QString declare( const QShaderDescription::VariableType type, QString name )
{
    using T = QShaderDescription::VariableType;
    switch ( type )
    {
        case T::Float:
            return QString( "float %1;" ).arg( name );
        case T::Vec2:
            return QString( "QVector2D %1;" ).arg( name );
        case T::Vec3:
            return QString( "QVector3D %1;" ).arg( name );
        case T::Vec4:
            return QString( "QVector4D %1;" ).arg( name );
        case T::Mat2:
            return "float [2][2]";
        case T::Mat2x3:
            return QString( "float %1[2][3];" ).arg( name );
        case T::Mat2x4:
            return QString( "float %1[2][4];" ).arg( name );
        case T::Mat3:
            return QString( "float %1[3][3];" ).arg( name );
        case T::Mat3x2:
            return QString( "float %1[3][2];" ).arg( name );
        case T::Mat3x4:
            return QString( "float %1[3][4];" ).arg( name );
        case T::Mat4:
            return QString( "float %1[4][4];" ).arg( name );
        case T::Mat4x2:
            return QString( "float %1[4][2];" ).arg( name );
        case T::Mat4x3:
            return QString( "float %1[4][3];" ).arg( name );
        case T::Int:
            return QString( "int %1[4][3];" ).arg( name );
        case T::Int2:
        case T::Int3:
        case T::Int4:
        case T::Uint:
        case T::Uint2:
        case T::Uint3:
        case T::Uint4:
        case T::Bool:
        case T::Bool2:
        case T::Bool3:
        case T::Bool4:
        case T::Double:
        case T::Double2:
        case T::Double3:
        case T::Double4:
        case T::DMat2:
        case T::DMat2x3:
        case T::DMat2x4:
        case T::DMat3:
        case T::DMat3x2:
        case T::DMat3x4:
        case T::DMat4:
        case T::DMat4x2:
        case T::DMat4x3:
        default:
            return QString( "void %1;" ).arg( name );
    }
}

int main( int argc, char* argv[] )
{
    enum CLI
    {
        Self,
        Qsb,
        Hpp,
        Count
    };

    QFile qsb( argv[ Qsb ] );
    QFile hpp( argv[ Hpp ] );

    if ( !qsb.open( QFile::ReadOnly ) )
    {
        qFatal() << "failed to open:" << argv[ Qsb ];
    }
    if ( !hpp.open( QFile::WriteOnly ) )
    {
        qFatal() << "failed to open:" << argv[ Hpp ];
    }

    const QFileInfo fileInfo( argv[ Qsb ] );
    qDebug() << fileInfo.fileName();

    const auto ns = fileInfo.fileName().replace( ".", "::" );
    const auto ig = fileInfo.fileName().replace( ".", "_" ).toUpper();

    const auto bytes = qsb.readAll();
    const auto shader = QShader::fromSerialized( bytes );
    const auto description = shader.description();

    const auto uniformBlocks = description.uniformBlocks();

    QTextStream os( &hpp );
    os << "// THIS FILE IS GENERATED!" << '\n';
    os << "#ifndef " << ig << '\n';
    os << "#define " << ig << '\n';
    os << "namespace " << ns << '\n';
    os << "{" << '\n';

    for ( const auto& block : qAsConst( uniformBlocks ) )
    {
        os << "struct " << block.structName << '\n';
        os << "{" << '\n';
        for ( const auto& member : qAsConst( block.members ) )
        {
            using T = QShaderDescription::VariableType;
            static QMap< T, QString > type{ { T::Vec2, "QVector2D" }, { T::Vec3, "QVector3D" },
                { T::Vec4, "QVector4D" } };
            qDebug() << member.name << member.size << member.offset;

            os << declare( member.type, member.name) << '\n';
        }

        // generate type trait
        os << '\n';
        os << "template<typename T>" << '\n';
        os << "struct is_layout_compatible" << '\n';
        os << "{" << '\n';
        os << "static constexpr bool value = sizeof(" << block.structName << ") == sizeof(T)" << '\n';
        for ( const auto& m : qAsConst( block.members ) ) 
        {
            os << "\t&& sizeof(" << m.name << ") == sizeof(T::" << m.name << ")" << '\n';
        }
        for ( const auto& m : qAsConst( block.members ) ) 
        {
            os << "\t&& offsetof(" << block.structName << ", " << m.name
               << ") == offsetof(T, " << m.name << ")" << '\n';
        }
        os << ';' << '\n';
        os << "};" << '\n';
        os << '\n';
        os << "template<typename T>" << '\n';
        os << "static constexpr bool is_layout_compatible_v = is_layout_compatible<T>::value;"
           << '\n';
        os << "};" << '\n';
    }

    os << "}" << '\n';
    os << "#endif" << '\n';

    return 0;
}