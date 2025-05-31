/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kizuna <kizuna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/25 20:00:00 by kizuna            #+#    #+#             */
/*   Updated: 2025/05/30 19:31:20 by kizuna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_ast(t_ast_node *ast, t_minishell *shell)
{
	if (!ast)
		return (0);
	if (ast->type == NODE_COMMAND)
		return (execute_command(ast, shell));
	else if (ast->type == NODE_PIPE)
		return (execute_pipeline(ast, shell));
	else if (ast->type >= NODE_REDIRECT_IN
		&& ast->type <= NODE_REDIRECT_HEREDOC)
		return (execute_redirections(ast, shell));
	else if (ast->type == NODE_AND || ast->type == NODE_OR
		|| ast->type == NODE_SEMICOLON)
		return (execute_logical_ops(ast, shell));
	return (0);
}

static int	execute_external_command(char *path, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		execve(path, args, envp);
		perror_exit("execve");
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

static int	handle_builtin_cmd(char **expanded_args, t_minishell *shell)
{
	int	exit_status;

	exit_status = execute_builtin(expanded_args, shell);
	free_args(expanded_args);
	return (exit_status);
}

static int	handle_external_cmd(char **expanded_args, t_minishell *shell)
{
	char	*path;
	int		exit_status;

	path = find_command_path(expanded_args[0], shell);
	if (!path)
	{
		exit_status = check_file_access(expanded_args[0]);
		if (exit_status == 127)
		{
			if (ft_strchr(expanded_args[0], '/'))
				print_error(expanded_args[0], "No such file or directory");
			else
				print_error(expanded_args[0], "command not found");
		}
		free_args(expanded_args);
		return (exit_status);
	}
	exit_status = execute_external_command(path, expanded_args, shell->envp);
	free(path);
	free_args(expanded_args);
	return (exit_status);
}

int	execute_command(t_ast_node *node, t_minishell *shell)
{
	char	**expanded_args;

	if (!node)
		return (0);
	if (!node->args || !node->args[0])
		return (0);
	expanded_args = expand_args(node->args, shell);
	if (!expanded_args)
		return (1);
	if (!expanded_args[0] || !*expanded_args[0])
	{
		free_args(expanded_args);
		return (0);
	}
	if (is_builtin(expanded_args[0]))
		return (handle_builtin_cmd(expanded_args, shell));
	return (handle_external_cmd(expanded_args, shell));
}
