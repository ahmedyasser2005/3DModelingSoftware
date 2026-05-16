#pragma once
#include <memory>

/**
 * @class Application
 * @brief High-level application controller class managing runtime subsystem components.
 * * Orchestrates loop processing, window events, updates, and frame generation passes.
 */
class [[nodiscard]] Application
{
  public:
    /**
     * @brief Instantiates internal component architectures and registers system loops.
     */
    Application();

    /**
     * @brief Dismantles application layers and safely flushes underlying subsystem resources.
     */
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;
    Application(Application &&) noexcept = delete;
    Application &operator=(Application &&) noexcept = delete;

    /**
     * @brief Launches the synchronized rendering and application ticking update pump sequence.
     * @return Application execution code return state integer (Typically zero on success).
     */
    [[nodiscard]] int Run();

  private:
    struct AppImpl;
    std::unique_ptr<AppImpl> m_impl;
};