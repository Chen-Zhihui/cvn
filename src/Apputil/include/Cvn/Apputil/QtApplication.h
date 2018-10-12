//
// Created by tom on 16-11-12.
//

#ifndef BASEAPP_QTSUBSYSTEM_H
#define BASEAPP_QTSUBSYSTEM_H

#include <Poco/Util/Application.h>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <memory>

namespace Cvn
{
namespace Apputil
{

class QtApplication : public Poco::Util::Application
{
  public:
    QtApplication() = default;

    ///   - initialize() (the one-argument, protected variant)
    ///   - uninitialize()
    ///   - reinitialize()
    ///   - defineOptions()
    ///   - handleOption()
    ///   - main()

    QApplication *app() { return _app.get(); }

    void init(int argc, char *argv[]);

  protected:
    void initialize(Application &self) override;

    void uninitialize() override;

    void reinitialize(Poco::Util::Application &self);

    int main(const std::vector<std::string> &args) override;

    virtual void setupMainUi();

    virtual void shotdownMainUi();

    void defineOptions(Poco::Util::OptionSet &options) override;

    void handleHelp(const std::string &name, const std::string &value);

    void handleDefine(const std::string &name, const std::string &value);

    void handleConfig(const std::string &name, const std::string &value);

    void displayHelp();

    void defineProperty(const std::string &def);

    void printProperties(const std::string &base);

  private:
    bool _helpRequested = false;
    int _argc;
    char **_argv;
    std::shared_ptr<QMainWindow> _main;
    std::shared_ptr<QApplication> _app;
};

} // namespace Apputil
} // namespace Cvn
#endif //BASEAPP_QTSUBSYSTEM_H
